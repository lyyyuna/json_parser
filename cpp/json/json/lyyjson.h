#ifndef LYY_JSON_H
#define LYY_JSON_H

#include <string>
#include <assert.h>
#include <cctype>
#include <cstdlib>

namespace lyy
{
	enum class JsonType
	{
		JNULL,		// null
		FALSE,
		TRUE,
		NUMBER,		// support E/e
		STRING,
		ARRAY,
		OBJECT
	};

	struct JsonValue
	{
		JsonType type;
		double num;
	};

	struct JsonContext
	{
		const char* json;
	};

	enum class ParseRet
	{
		PARSE_OK,
		PARSE_EXCEPT_VALUE, // empty
		PARSE_INVALID_VALUE,
		PARSE_ROOT_NOT_SINGULAR, // still char after
		PARSE_NUMBER_TOO_BIG	// too big number
	};

	struct JsonParser
	{
		static ParseRet		parse(JsonValue& v, const char* json);
		static ParseRet		parse_value(JsonContext& c, JsonValue& v);
		static ParseRet		parse_null(JsonContext& c, JsonValue& v);
		static ParseRet		parse_true(JsonContext& c, JsonValue& v);
		static ParseRet		parse_false(JsonContext& c, JsonValue& v);
		static ParseRet		parse_number(JsonContext& c, JsonValue& v);

		static void			parse_whitespace(JsonContext& c);
		static void			next(JsonContext& c, char ch) { assert(*c.json == (ch)); c.json++; }
		static JsonType     get_type(JsonValue& v) { return v.type; }
	};


}

#endif