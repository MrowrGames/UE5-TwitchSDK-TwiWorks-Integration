// We silence this warning because this functionality works fine and there is no reasonable
// replacement right now. The only alternative is to reimplement wstring_convert.
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1

#include <codecvt>
#include <locale>
#include "r66_json.hpp"


// Work around STL bug: covdecvt has a protected (???) destructor according to the standard
// https://stackoverflow.com/a/41744847
template<class I, class E, class S>
struct codecvt : std::codecvt<I, E, S>
{
    ~codecvt()
    { }
};



R66::string_holder R66::from_utf8(std::string_view s)
{
#if defined(R66_STR_WSTRING) || defined(R66_STR_U16STRING)
	std::wstring_convert<codecvt<r66char, char, std::mbstate_t>, r66char> convert;
	return convert.from_bytes(s.data(), s.data() + s.size());
#else
	return s;
#endif
}

R66::stdstring_holder R66::to_utf8(string_view s)
{
#if defined(R66_STR_WSTRING) || defined(R66_STR_U16STRING)
	std::wstring_convert<codecvt<r66char, char, std::mbstate_t>, r66char> convert;
	return convert.to_bytes(s.data(), s.data() + s.size());
#else
	return s;
#endif
}
