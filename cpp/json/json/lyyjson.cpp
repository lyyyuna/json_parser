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
		default: return ParseRet::PARSE_INVALID_VALUE;
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
}