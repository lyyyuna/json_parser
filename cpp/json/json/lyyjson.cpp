#include "lyyjson.h"

namespace lyy
{
	/*
	* main parse
	*/
	JsonValue::Ptr JsonParser::parse(const char* json, ParseRet& ret)
	{
		auto c = JsonContext::Ptr(new JsonContext(json));

		// consume possible whitespace
		parse_whitespace(c);
		auto v = parse_value(c, ret);
		if (ParseRet::PARSE_OK == ret)
		{
			parse_whitespace(c);
			if (*c->json != '\0')
				ret = ParseRet::PARSE_ROOT_NOT_SINGULAR;
		}
		return v;
	}

	JsonValue::Ptr JsonParser::parse_value(JsonContext::Ptr c, ParseRet& ret)
	{
		switch (*c->json)
		{
		case 'n': return parse_null(c, ret);
		case 't': return parse_true(c, ret);
		case 'f': return parse_false(c, ret);
		case '\"': return parse_string(c, ret);
		case '\0':
			ret = ParseRet::PARSE_EXCEPT_VALUE;
			return JsonValue::Ptr(new JsonValue());
		default: return parse_number(c, ret);
		}
	}

	/*
	* consume as much whitespace as possible
	*/
	void JsonParser::parse_whitespace(JsonContext::Ptr c)
	{
		auto p = c->json;
		while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
		{
			p++;
		}
		c->json = p;
	}

	/*
	* parse null
	*/
	JsonValue::Ptr JsonParser::parse_null(JsonContext::Ptr c, ParseRet& ret)
	{
		auto v = JsonValue::Ptr(new JsonValue());

		next(c, 'n');
		if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
		{
			ret = ParseRet::PARSE_INVALID_VALUE;
			return v;
		}
		c->json += 3;
		ret = ParseRet::PARSE_OK;
		return v;
	}

	/*
	* parse true
	*/
	JsonValue::Ptr JsonParser::parse_true(JsonContext::Ptr c, ParseRet& ret)
	{
		next(c, 't');
		if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		{
			ret = ParseRet::PARSE_INVALID_VALUE;
			return JsonValue::Ptr(new JsonValue());;
		}

		auto v = JsonValue::Ptr(new JsonValue(true));
		c->json += 3;
		ret = ParseRet::PARSE_OK;
		return v;
	}

	/*
	* parse false
	*/
	JsonValue::Ptr JsonParser::parse_false(JsonContext::Ptr c, ParseRet& ret)
	{
		next(c, 'f');
		if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
		{
			ret = ParseRet::PARSE_INVALID_VALUE;
			return JsonValue::Ptr(new JsonValue());;
		}

		auto v = JsonValue::Ptr(new JsonValue(false));
		c->json += 4;
		ret = ParseRet::PARSE_OK;
		return v;
	}

	/*
	* parse number
	*/
	JsonValue::Ptr JsonParser::parse_number(JsonContext::Ptr c, ParseRet& ret)
	{
		const char* tmp = c->json;

		if (*tmp == '-') tmp++;
		if (*tmp == '0') tmp++;
		else {
			if (!isdigit(*tmp) || '0' == *tmp)
			{
				ret = ParseRet::PARSE_INVALID_VALUE;
				return JsonValue::Ptr(new JsonValue());
			}
			for (tmp++; isdigit(*tmp); ++tmp) {}
		}
		if (*tmp == '.')
		{
			tmp++;
			if (!isdigit(*tmp))
			{
				ret = ParseRet::PARSE_INVALID_VALUE;
				return JsonValue::Ptr(new JsonValue());
			}
			for (tmp++; isdigit(*tmp); ++tmp) {}
		}
		if (*tmp == 'e' || *tmp == 'E')
		{
			tmp++;
			if (*tmp == '+' || *tmp == '-')
				tmp++;
			if (!isdigit(*tmp))
			{
				ret = ParseRet::PARSE_INVALID_VALUE;
				return JsonValue::Ptr(new JsonValue());
			}
			for (tmp++; isdigit(*tmp); ++tmp) {}
		}

		errno = 0;
		double num = strtod(c->json, NULL);
		if (errno == ERANGE)
		{
			if (num == HUGE_VAL || num == -HUGE_VAL)
			{
				ret = ParseRet::PARSE_NUMBER_TOO_BIG;
				return JsonValue::Ptr(new JsonValue());
			}
		}

		auto v = JsonValue::Ptr(new JsonValue(num));
		c->json = tmp;
		ret = ParseRet::PARSE_OK;
		return v;
	}

	/*
	* 
	*/
	const char* JsonParser::parse_hex4(const char* p, unsigned* u)
	{
		*u = 0;
		for (int i = 0; i < 4; ++i)
		{
			char ch = *p++;
			*u <<= 4;
			if (ch >= '0' && ch <= '9')  *u |= ch - '0';
			else if (ch >= 'A' && ch <= 'F')  *u |= ch - ('A' - 10);
			else if (ch >= 'a' && ch <= 'f')  *u |= ch - ('a' - 10);
			else return NULL;
		}
		return p;
	}

	/*
	*
	*/
	void JsonParser::encode_utf8(JsonContext::Ptr c, unsigned u, JsonValue::Str& tmpstr)
	{
		if (u <= 0x7f)
			tmpstr.push_back(u & 0xff);
		else if (u <= 0x7ff) {
			tmpstr.push_back(0xC0 | ((u >> 6)	& 0xFF));
			tmpstr.push_back(0x80 | (u			& 0x3F));
		}
		else if (u <= 0xffff) {
			tmpstr.push_back(0xE0 | ((u >> 12) & 0xFF));
			tmpstr.push_back(0x80 | ((u >> 6) & 0x3F));
			tmpstr.push_back(0x80 | (u & 0x3F));
		}
		else {
			assert(u <= 0x10FFFF);
			tmpstr.push_back(0xF0 | ((u >> 18) & 0xFF));
			tmpstr.push_back(0x80 | ((u >> 12) & 0x3F));
			tmpstr.push_back(0x80 | ((u >> 6) & 0x3F));
			tmpstr.push_back(0x80 | (u & 0x3F));
		}
	}

	/*
	* parse string
	*/
	JsonValue::Ptr JsonParser::parse_string(JsonContext::Ptr c, ParseRet& ret)
	{
		next(c, '\"');
		auto tmp = c->json;
		JsonValue::Str tmpstr;

		while (true)
		{
			char ch = *tmp++;
			switch (ch)
			{
			case '\"': {
				auto v = JsonValue::Ptr(new JsonValue(tmpstr));
				c->json = tmp;
				ret = ParseRet::PARSE_OK;
				return v;
			}
			case '\\': {
				switch (*tmp++)
				{
					case '\"': tmpstr.push_back('\"'); break;
					case '\\': tmpstr.push_back('\\'); break;
					case '/': tmpstr.push_back('/'); break;
					case 'f': tmpstr.push_back('\f'); break;
					case 'n': tmpstr.push_back('\n'); break;
					case 'b': tmpstr.push_back('\b'); break;
					case 'r': tmpstr.push_back('\r'); break;
					case 't': tmpstr.push_back('\t'); break;
					case 'u': {
						unsigned u, u2;
						if (!(tmp = parse_hex4(tmp, &u)))
						{
							auto v = JsonValue::Ptr(new JsonValue());
							ret = ParseRet::PARSE_INVALID_UNICODE_HEX;
							return v;
						}
						if (u >= 0xD800 && u <= 0xDBFF)
						{
							if (*tmp++ != '\\')
							{
								auto v = JsonValue::Ptr(new JsonValue());
								ret = ParseRet::PARSE_INVALID_UNICODE_SURROGATE;
								return v;
							}
							if (*tmp++ != 'u')
							{
								auto v = JsonValue::Ptr(new JsonValue());
								ret = ParseRet::PARSE_INVALID_UNICODE_SURROGATE;
								return v;
							}
							if (!(tmp = parse_hex4(tmp, &u2)))
							{
								auto v = JsonValue::Ptr(new JsonValue());
								ret = ParseRet::PARSE_INVALID_UNICODE_HEX;
								return v;
							}
							if (u2 < 0xDC00 || u2 > 0xDFFF)
							{
								auto v = JsonValue::Ptr(new JsonValue());
								ret = ParseRet::PARSE_INVALID_UNICODE_SURROGATE;
								return v;
							}
							u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
						}
						encode_utf8(c, u, tmpstr);
						break;
					}
					default: {
						auto v = JsonValue::Ptr(new JsonValue());
						ret = ParseRet::PARSE_INVALID_STRING_ESCAPE;
						return v;
					}
				}
				break;
			}
			case '\0': {
				auto v = JsonValue::Ptr(new JsonValue());
				ret = ParseRet::PARSE_MISS_QUOTATION_MARK;
				return v;
			}
			default:
				if ((unsigned char)ch < 0x20)
				{
					auto v = JsonValue::Ptr(new JsonValue());
					ret = ParseRet::PARSE_INVALID_STRING_ESCAPE;
					return v;
				}
				tmpstr.push_back(ch);
				break;
			}
		}
	}
}