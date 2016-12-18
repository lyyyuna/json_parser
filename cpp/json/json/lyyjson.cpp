#include "lyyjson.h"

namespace lyy
{
	ParseRet JsonParser::parse(JsonValue& v, const char* json)
	{
		JsonContext c;
		c.json = json;
		v.type = JsonType::JNULL;
		// consume whitespace
		parse_whitespace(c);
		auto ret = parse_value(c, v);
		if (ParseRet::PARSE_OK == ret)
		{
			// consume whitespace
			parse_whitespace(c);
			if (*c.json != '\0')
			{
				ret = ParseRet::PARSE_ROOT_NOT_SINGULAR;
			}
		}
		return ret;
	}

	ParseRet JsonParser::parse_value(JsonContext& c, JsonValue& v)
	{
		switch (*c.json)
		{
		case 't': return parse_true(c, v);
		case 'f': return parse_false(c, v);
		case 'n': return parse_null(c, v);
		case '\0': return ParseRet::PARSE_EXCEPT_VALUE;
		default: return parse_number(c, v);
		}
	}

	// Ignore whitespace
	void JsonParser::parse_whitespace(JsonContext& c)
	{
		auto p = c.json;
		while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
		{
			p++;
		}
		c.json = p;
	}

	ParseRet JsonParser::parse_true(JsonContext& c, JsonValue& v)
	{
		next(c, 't');
		if (c.json[0] != 'r' || c.json[1] != 'u' || c.json[2] != 'e')
		{
			return ParseRet::PARSE_INVALID_VALUE;
		}
		c.json += 3;
		v.type = JsonType::TRUE;
		return ParseRet::PARSE_OK;
	}

	ParseRet JsonParser::parse_false(JsonContext& c, JsonValue& v)
	{
		next(c, 'f');
		if (c.json[0] != 'a' || c.json[1] != 'l' || c.json[2] != 's' || c.json[3] != 'e')
		{
			return ParseRet::PARSE_INVALID_VALUE;
		}
		c.json += 4;
		v.type = JsonType::FALSE;
		return ParseRet::PARSE_OK;
	}

	ParseRet JsonParser::parse_null(JsonContext& c, JsonValue& v)
	{
		next(c, 'n');
		if (c.json[0] != 'u' || c.json[1] != 'l' || c.json[2] != 'l')
		{
			return ParseRet::PARSE_INVALID_VALUE;
		}
		c.json += 3;
		v.type = JsonType::JNULL;
		return ParseRet::PARSE_OK;
	}

	ParseRet JsonParser::parse_number(JsonContext& c, JsonValue& v)
	{
		const char* tmp = c.json;

		if (*tmp == '-') tmp++;
		if (*tmp == '0') tmp++; // see spec
		else {
			if (!isdigit(*tmp) || '0' == *tmp) // digit should be 1-9 as first character
				return ParseRet::PARSE_INVALID_VALUE;
			for (tmp++; isdigit(*tmp); ++tmp)
			{ }
		}
		if (*tmp == '.')
		{
			tmp++;
			if (!isdigit(*tmp))
				return ParseRet::PARSE_INVALID_VALUE;
			for (tmp++; isdigit(*tmp); ++tmp)
			{ }
		}
		if (*tmp == 'e' || *tmp == 'E')
		{
			tmp++;
			if (*tmp == '+' || *tmp == '-')
				tmp++;
			if (!isdigit(*tmp))
				return ParseRet::PARSE_INVALID_VALUE;
			for (tmp++; isdigit(*tmp); ++tmp)
			{ }
		}

		errno = 0;
		v.num = strtod(c.json, NULL);
		if (errno == ERANGE)
		{
			if (v.num == HUGE_VAL || v.num == -HUGE_VAL)
			{
				return ParseRet::PARSE_NUMBER_TOO_BIG;
			}
			// may not happen...
			// for 1e-10000 this kind of number, errno is also out of range
			// but return 0, i think it is also OK, due to extremely too small number.
			// return ParseRet::PARSE_INVALID_VALUE;
		}
		v.type = JsonType::NUMBER;
		c.json = tmp; // move the pointer
		return ParseRet::PARSE_OK;
	}
}
