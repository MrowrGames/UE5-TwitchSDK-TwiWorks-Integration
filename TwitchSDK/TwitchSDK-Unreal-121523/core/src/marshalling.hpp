#pragma once
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include "r66_strings.hpp"
#include "pal.hpp"


namespace R66::Marshalling {
	static_assert(std::is_same_v<r66char, char16_t>, "The C# marshalling code only works when compiling with UTF-16 strings!");

	typedef void (*GenericCallbackHandler)(void* payload, void* retval);

	class Marshallable {
	protected:
		int32_t TypeCode;

		Marshallable(int32_t code) : TypeCode(code) {}
	};
	void ConsumeLambda(void* p, void* arg);
	template<class TRet> class MarshallableStartAsync : public Marshallable {
		GenericCallbackHandler TaskCallback;
		void* TaskCallbackPayload;

	protected:
		MarshallableStartAsync(
			int32_t code,
			std::function<void(const TRet&)> cb,
			std::function<void(const std::exception&)> rej
		);
	};

	class StringManaged {
		const char16_t* Data;
		int32_t Len;
	public:
		StringManaged(std::u16string_view s) : Data(s.data()) {
			auto size = s.size();
			if (size > INT32_MAX) throw std::range_error("string too long");
			Len = static_cast<int32_t>(size);
		}
		operator std::u16string_view() const {
			return std::u16string_view(Data, Len);
		}
		operator R66::u16string_holder() const {
			return std::u16string_view(*this);
		}
	};

	class StringArrayManaged;
	class StringArrayManagedIter {
		const StringArrayManaged& Parent;
		size_t Index;
		friend StringArrayManaged;
		StringArrayManagedIter(const StringArrayManaged& p, size_t i) : Parent(p), Index(i) {}
	public:
		bool operator !=(const StringArrayManagedIter& other);
		StringArrayManagedIter operator++();
		std::u16string_view operator*();
	};
	class StringArrayManaged {
		friend StringArrayManagedIter;
		const char16_t** Data;
		int32_t* Lengths;

	public:
		explicit StringArrayManaged(const std::vector<R66::u16string_holder>& v);
		StringArrayManaged(const StringArrayManaged&) = delete;
		~StringArrayManaged();

		int32_t Length() const {
			return Lengths ? *Lengths : 0;
		}
		operator std::vector<R66::string_holder>() const;

		StringArrayManagedIter begin() const {
			return StringArrayManagedIter(*this, 0);
		}
		StringArrayManagedIter end() const {
			return StringArrayManagedIter(*this, Length());
		}
	};

	class ManagedStreamInfo;
	template<class T> class GenericArrayManaged {
		T* Data;
		int32_t Length;

	public:
		template<class U> explicit GenericArrayManaged(const std::vector<U>& v)
			: Data((T*)std::malloc(sizeof(T) * v.size())), Length(static_cast<int32_t>(v.size())) {
			if (v.size() > INT32_MAX) throw std::range_error("generic array too long");

			for (int32_t i = 0; i < Length; i++) {
				new (Data + i) T(v[i]);
			}
		}

		~GenericArrayManaged() {
			for (int32_t i = 0; i < Length; i++) {
				Data[i].~T();
			}
			std::free(Data);
		}
		template<class U> operator std::vector<U>() const
		{
			std::vector<U> res;
			for (int32_t i = 0; i < Length; i++)
			{
				res.emplace_back(Data[i]);
			}
			return res;
		}
	};
}

#include "marshalling_autogenerated.hpp"

// Must declare it like this as we are not allowed to reference ManagedMarshalException::TYPECODE inline above
template<class TRet> inline R66::Marshalling::MarshallableStartAsync<TRet>::MarshallableStartAsync(
	int32_t code,
	std::function<void(const TRet&)> cb,
	std::function<void(const std::exception&)> rej
) : Marshallable(code) {
	TaskCallback = ConsumeLambda;
	TaskCallbackPayload = (void*)new std::function<void(void*)>(
		[cb, rej](void* p) {
			if (!p) {
				rej(std::runtime_error("null returned"));
				return;
			}

			int32_t typecode = *(const int32_t*)p;
			if (typecode == TRet::TYPECODE)
			{
				try {
					cb(*(const TRet*)p);
				}
				catch (const std::exception& e)
				{
					rej(e);
				}
			}
			else if (typecode == ManagedMarshalException::TYPECODE)
			{
				auto ex = (const ManagedMarshalException*)p;
				auto what = to_utf8(ex->What).ToOwned();
				rej(std::runtime_error(what));
			}
			else
			{
				rej(std::runtime_error("wrong type returned"));
			}
		}
	);
}
