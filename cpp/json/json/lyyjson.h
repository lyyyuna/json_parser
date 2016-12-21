#ifndef LYY_JSON_H
#define LYY_JSON_H

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <cctype>
#include <cstdlib>

using namespace std;

namespace lyy
{
	enum class JsonType
	{
		JNULL,
		JString,
		JTRUE,
		JFALSE,
		JNUMBER,
		JARRAY,
		JOBJECT
	};

	enum class ParseRet
	{
		PARSE_OK,
		PARSE_EXCEPT_VALUE, // empty
		PARSE_INVALID_VALUE,
		PARSE_ROOT_NOT_SINGULAR, // still char after
		PARSE_NUMBER_TOO_BIG	// too big number
	};

	enum class ValueRet
	{
		OK,
		ERR
	};

	class JsonContext
	{
	public:
		typedef shared_ptr<JsonContext>		Ptr;
		JsonContext();
		JsonContext(const char* json);


		const char* json;
	};

	struct JsonValue
	{
		typedef shared_ptr<JsonValue>	Ptr;
		typedef shared_ptr<string>		StrPtr;

	public:
		JsonValue();	// default as JNULL
		JsonValue(bool bool_value);
		JsonValue(double double_value);

		JsonType get_type();
		ValueRet get_value(double& number_value);
		ValueRet get_value(string& string_value);

	private:
		JsonType type;
		union {
			double number_value;
			string* string_value;
		};

	};

	struct JsonParser
	{
		static JsonValue::Ptr		parse(const char* json, ParseRet& ret);
		static JsonValue::Ptr		parse_value(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_null(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_true(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_false(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_number(JsonContext::Ptr c, ParseRet& ret);

		static void					parse_whitespace(JsonContext::Ptr c);
		static void					next(JsonContext::Ptr c, char ch) { assert(*c->json == (ch)); c->json++; }
	};
}

#endif
