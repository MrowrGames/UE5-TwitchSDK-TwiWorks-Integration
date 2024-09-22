#include <stdexcept>
#include <climits>

#include "urlencode.hpp"

using namespace R66;

static void write_hex_letter(string& dst, char i)
{
	r66char c;
	if (i <= 0x9)
	{
		c = STR('0') + i;
	}
	else if (i <= 0xf)
	{
		c = STR('A') + i - 10;
	}
	else
	{
		throw std::out_of_range("invalid char");
	}
	dst += static_cast<r66char>(c);
}
void R66::EncodeUriComponent(string& dst, string_view s)
{
	auto utf8 = to_utf8(s);
	for (unsigned char c : (std::string_view)utf8)
	{
		switch (c)
		{
		case ' ':
			dst += STR('+');
			break;

		case '-':
		case '_':
		case '.':
		case '~':
		alphanum:
			dst += static_cast<r66char>(c);
			break;

		default:
			if ((c <= CHAR_MAX) && isalnum(static_cast<unsigned char>(c)))
			{
				goto alphanum;
			}

			// Ideally we could simply do this:
			// dst << std::uppercase << std::setw(2) << std::setfill(STR('0')) << std::hex << static_cast<uint32_t>(c);
			// Sadly, that is broken on MSVC with char16_t so we have to implement it ourselves
			dst += '%';
			write_hex_letter(dst, c >> 4);
			write_hex_letter(dst, c & 0xf);
		}
	}
}
