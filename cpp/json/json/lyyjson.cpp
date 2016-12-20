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
		case '\0':
			ret = ParseRet::PARSE_EXCEPT_VALUE;
			return JsonValue::Ptr(new JsonValue());
		default:
			ret = ParseRet::PARSE_INVALID_VALUE;
			return JsonValue::Ptr(new JsonValue());
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
}