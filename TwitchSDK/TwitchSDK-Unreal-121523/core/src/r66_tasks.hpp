#pragma once
#include <functional>
#include <memory>
#include <list>
#include <variant>
#include <exception>
#include <queue>
#include <utility>
#include <map>


namespace R66 {
	template<class T> class CatchResolveResult {
		ResolveFn<const T&> Resolve;
		RejectFn Reject;

	public:
		CatchResolveResult(ResolveFn<const T&>&& resolve, RejectFn&& reject) : Resolve(resolve), Reject(reject) {}
		void operator()(const T& t) const {
			try {
				Resolve(t);
			}
			catch (const std::exception& e) {
				Reject(e);
			}
		}
	};

	template<class TResult> class InfallibleTask {
		using CbList = std::list<ResolveFn<const TResult&>>;
		using ResType = std::variant<std::monostate, TResult>;
		CbList Callbacks;
		ResType Result;

	public:
		void Resolve(TResult&& ai)
		{
			// See comment above in FallibleTask::Resolve
			auto& cbs = Callbacks;
			while (!cbs.empty())
			{
				auto cb = std::move(cbs.front());
				cbs.pop_front();
				cb(ai);
			}

			Result = ai;
		}
		const TResult& Unwrap() const
		{
			return std::visit([](auto& arg) -> const TResult&
				{
					using T = std::decay_t<decltype(arg)>;

					if constexpr (std::is_same_v<T, std::monostate>) {
						throw std::runtime_error("task is still pending");
					}
					else {
						return arg;
					}
				}, Result);
		}
		TResult& UnwrapMut()
		{
			return std::visit([](auto& arg) -> TResult&
				{
					using T = std::decay_t<decltype(arg)>;

					if constexpr (std::is_same_v<T, std::monostate>) {
						throw std::runtime_error("task is still pending");
					}
					else {
						return arg;
					}
				}, Result);
		}
		bool IsDone() const
		{
			return std::holds_alternative<TResult>(Result);
		}
		void Invalidate()
		{
			Result = std::monostate();
		}

		bool Await(ResolveFn<const TResult&>&& resolve)
		{
			if (auto ai = std::get_if<TResult>(&Result))
			{
				resolve(*ai);
				return false;
			}

			bool is_first = Callbacks.empty();
			Callbacks.push_back(std::move(resolve));
			return is_first;
		}
		bool Await(ResolveFn<const TResult&>&& resolve, RejectFn&& reject)
		{
			return Await(CatchResolveResult<TResult>(std::move(resolve), std::move(reject)));
		}
	};
	template<class TResult> class FallibleTask {
		using CbList = std::list<std::pair<ResolveFn<const TResult&>, RejectFn>>;
		using ResType = std::variant<std::monostate, TResult, std::exception>;
		CbList Callbacks;
		ResType Result;

	public:
		void Resolve(TResult&& ai)
		{
			// This invokes callbacks which may call Await on this task which may add
			// callbacks to the other side of the same list we're currently processing.
			// That is intentional.
			// See ResolveSPMC below for details.
			auto& cbs = Callbacks;
			while (!Callbacks.empty())
			{
				auto cb = std::move(cbs.front());
				cbs.pop_front();
				try
				{
					cb.first(ai);
				}
				catch (const std::exception& e)
				{
					cb.second(e);
				}
			}

			Result = ai;
		}
		void Reject(const std::exception& e)
		{
			// See comment above in FallibleTask::Resolve
			auto& cbs = Callbacks;
			while (!cbs.empty())
			{
				auto cb = cbs.front();
				cb.second(e);
				cbs.pop_front();
			}

			Result = e;
		}
		const TResult& Unwrap() const
		{
			return std::visit([](auto& arg) -> const TResult&
				{
					using T = std::decay_t<decltype(arg)>;

					if constexpr (std::is_same_v<T, std::monostate>) {
						throw std::runtime_error("task is still pending");
					}
					else if constexpr (std::is_same_v<T, std::exception>) {
						throw arg;
					}
					else {
						return arg;
					}
				}, Result);
		}
		bool IsSuccess() const
		{
			return std::holds_alternative<TResult>(Result);
		}
		void Invalidate()
		{
			Result = std::monostate();
		}

		bool Await(
			ResolveFn<const TResult&>&& resolve,
			RejectFn&& reject
		)
		{
			if (auto ai = std::get_if<TResult>(&Result))
			{
				resolve(*ai);
				return false;
			}
			if (auto e = std::get_if<std::exception>(&Result))
			{
				reject(*e);
				return false;
			}

			bool is_first = Callbacks.empty();
			Callbacks.push_back(std::make_pair(std::move(resolve), std::move(reject)));
			return is_first;
		}

		// The SPMC variants assume a model where we (essentially) invalidate ourselves
		// whenever we deliver a result to a consumer.
		void AwaitSPMC(ResolveFn<const TResult&>&& resolve, RejectFn&& reject)
		{
			// we swap out the result so that reentrant AwaitSPMC calls further up the
			// call stack won't be able to see the result
			ResType myResult = std::monostate();
			std::swap(myResult, Result);
			if (auto ai = std::get_if<TResult>(&myResult))
			{
				resolve(*ai);
			}
			else
			{
				Callbacks.push_back(std::make_pair(std::move(resolve), std::move(reject)));
			}
		}
		void ResolveSPMC(TResult&& ai)
		{
			// In the ResolveSPMC case it's important that we do not iterate over the live Callbacks list
			// and instead swap it out. The reason is that here we want to return one item at a time.
			// For a consumer who immediately re-invokes AwaitSPMC inside an await callback (i.e. while
			// ResolveSPMC is still on the stack) adding the new callback to the list and immediately processing
			// it would create an infinite loop where the same value is returned over and over.
			// Unreal does this.
			CbList cbs;
			std::swap(cbs, Callbacks);
			if (cbs.empty())
			{
				Result = ai;
				return;
			}

			do
			{
				auto cb = cbs.front();
				try
				{
					cb.first(ai);
				}
				catch (const std::exception& e)
				{
					cb.second(e);
				}
				cbs.pop_front();
			}
			while (!cbs.empty());

			// simply discard Result on this path since a consumer has seen it
		}
	};


	template<class T> class MulticastEventQueue;
	template<class T> class EventQueue {
		friend MulticastEventQueue<T>;
		std::queue<T> PendingEvents;
		std::queue<std::pair<ResolveFn<const T&>, RejectFn>> PendingQueries;
		TimePoint LastPoll;

	public:
		~EventQueue() {
			while (!PendingQueries.empty())
			{
				PendingQueries.front().second(std::runtime_error("terminating stream"));
				PendingQueries.pop();
			}
		}

		void Pop(ResolveFn<const T&>&& resolve, RejectFn&& reject) {
			LastPoll = SteadyClock::now();
			if (PendingEvents.empty())
			{
				PendingQueries.push(std::make_pair(std::move(resolve), std::move(reject)));
			}
			else
			{
				resolve(PendingEvents.front());
				PendingEvents.pop();
			}
		}
		void Push(const T& e) {
			if (PendingQueries.empty())
			{
				if (PendingEvents.size() >= 512)
				{
					// discard instead of leaking even more memory
					return;
				}
				PendingEvents.push(e);
			}
			else
			{
				try
				{
					PendingQueries.front().first(e);
				}
				catch (const std::exception& e)
				{
					PendingQueries.front().second(e);
				}
				PendingQueries.pop();
			}
		}
	};
	class IMulticastEventQueue {
	public:
		using TKey = int32_t;
		virtual ~IMulticastEventQueue() {}
		virtual TKey Register() = 0;
		virtual void Unregister(TKey k) = 0;
	};
	template<class T> class MulticastEventQueue : public IMulticastEventQueue {
		TKey NextKey = 0;
		std::map<TKey, EventQueue<T>> Queues;
		using QueueEle = typename decltype(Queues)::value_type;
	public:
		void Push(const T& e) {
			for (auto& pair : Queues)
			{
				pair.second.Push(e);
			}
		}
		void Pop(TKey key, ResolveFn<const T&>&& resolve, RejectFn&& reject) {
			auto queue = Queues.find(key);
			if (queue != Queues.end())
			{
				queue->second.Pop(std::move(resolve), std::move(reject));
			}
			else
			{
				reject(std::runtime_error("stream is closed (or key is just invalid)"));
			}
		}
		virtual TKey Register() override {
			TKey key = NextKey++;
			// 64 subscriptions per queue is the maximum we allow to avoid memory leaks
			while (Queues.size() >= 64)
			{
				// clear out the oldest
				auto oldest = std::min_element(Queues.begin(), Queues.end(), [](const QueueEle& a, const QueueEle& b) { return a.second.LastPoll < b.second.LastPoll; });
				Queues.erase(oldest);
			}
			Queues.try_emplace(key);
			return key;
		}
		virtual void Unregister(TKey k) override {
			Queues.erase(k);
		}
	};
}
