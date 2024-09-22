#pragma once
#include <exception>
#include <stdexcept>

namespace R66 {
	class JsonMissingKeyError : public std::out_of_range {
	public:
		explicit JsonMissingKeyError(const std::string& key)
			: std::out_of_range("Missing JSON key: " + key) {}
	};
	class JsonBadValueError : public std::runtime_error {
	public:
		explicit JsonBadValueError(const std::string& desired)
			: std::runtime_error("JSON value is not the desired type: " + desired) {}
	};
}

#ifndef R66_JSON_EXTERNAL
#include <cstdint>

#include "r66_strings.hpp"
#include <nlohmann/json.hpp>

namespace R66 {
	class JsonRef;
	class JsonBase {
		virtual const nlohmann::json& GetRef() const = 0;
	public:
		JsonRef at(const std::string& path) const;
		JsonRef at(std::size_t i) const;
		std::string_view to_std_string() const;
		int64_t to_int() const;
		bool to_bool() const;
		string to_string() const;
		size_t array_size() const;
		bool is_null() const;
	};
	class JsonRef : public JsonBase
	{
		friend JsonBase;
		const nlohmann::json& Reference;
		virtual const nlohmann::json& GetRef() const { return Reference; }
	public:
		explicit JsonRef(const nlohmann::json& j) : Reference(j) {}
	};
	class Json : public JsonBase {
		nlohmann::json JsonOwned;
		virtual const nlohmann::json& GetRef() const { return JsonOwned; }
	public:
		Json() : JsonOwned() {}
		explicit Json(string_view s) : JsonOwned(to_utf8(s)) {}
		explicit Json(std::string_view s) : JsonOwned(std::string(s)) {}
		explicit Json(const r66char* s) : JsonOwned(string_view(s)) {}
		explicit Json(const char* s) : JsonOwned(std::string_view(s)) {}
		explicit Json(int64_t i) : JsonOwned(i) {}
		explicit Json(int32_t i) : JsonOwned(i) {}
		explicit Json(bool i) : JsonOwned(i) {}

		static Json Parse(string_view s)
		{
			Json j;
			j.JsonOwned = nlohmann::json::parse(s);
			return j;
		}

		void set_property(const std::string& path, Json value)
		{
			JsonOwned[path] = value.JsonOwned;
		}
		void set_array(const std::string& path, std::vector<Json>&& data)
		{
			std::vector<nlohmann::json> v;
			v.reserve(data.size());
			for (auto&& j : data) {
				v.push_back(std::move(j.JsonOwned));
			}
			JsonOwned[path] = v;
		}

		string serialize() const
		{
			return from_utf8(JsonOwned.dump()).ToOwned();
		}
	};

	inline JsonRef JsonBase::at(const std::string& path) const
	{
		try {
			return JsonRef(GetRef().at(path));
		}
		catch (...)
		{
			throw JsonMissingKeyError(path);
		}
	}

	inline JsonRef JsonBase::at(std::size_t i) const
	{
		try {
			return JsonRef(GetRef().at(i));
		}
		catch (...)
		{
			throw JsonMissingKeyError(std::to_string(i));
		}
	}

	inline std::string_view JsonBase::to_std_string() const
	{
		try
		{
			return GetRef().get<std::string_view>();
		}
		catch (...)
		{
			throw JsonBadValueError("string");
		}
	}

	inline int64_t JsonBase::to_int() const
	{
		try
		{
			return GetRef().get<int64_t>();
		}
		catch (...)
		{
			throw JsonBadValueError("int");
		}
	}

	inline bool JsonBase::to_bool() const
	{
		try
		{
			return GetRef().get<bool>();
		}
		catch (...)
		{
			throw JsonBadValueError("bool");
		}
	}
	inline size_t JsonBase::array_size() const
	{
		auto& r = GetRef();
		if (!r.is_array())
			throw JsonBadValueError("array");
		return r.size();
	}
	inline bool JsonBase::is_null() const
	{
		auto& r = GetRef();
		return r.is_null();
	}

	inline string JsonBase::to_string() const
	{
		return from_utf8(to_std_string()).ToOwned();
	}
}

#endif
