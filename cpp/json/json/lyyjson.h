#ifndef LYY_JSON_H
#define LYY_JSON_H

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <map>

using namespace std;

namespace lyy
{
	enum class JsonType
	{
		JNULL,
		JSTRING,
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
		PARSE_NUMBER_TOO_BIG,	// too big number
		PARSE_MISS_QUOTATION_MARK,
		PARSE_INVALID_STRING_ESCAPE,
		PARSE_INVALID_STRING_CHAR,
		PARSE_INVALID_UNICODE_HEX,
		PARSE_INVALID_UNICODE_SURROGATE,
		PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
		PARSE_MISS_KEY,
		PARSE_MISS_COLON,
		PARSE_MISS_COMMA_OR_CURLY_BRACKET
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
		typedef shared_ptr<JsonValue>					Ptr;
		typedef vector<unsigned>						Str;
		typedef vector<JsonValue::Ptr>					Array;
		typedef map<Str, JsonValue::Ptr>		Object;

	public:
		JsonValue();	// default as JNULL
		JsonValue(bool bool_value);
		JsonValue(double double_value);
		JsonValue(Str& string_value);
		JsonValue(Array& array_value);
		JsonValue(Object& object_value);

		JsonType get_type();
		ValueRet get_value(double& number_value);
		ValueRet get_value(JsonValue::Str& string_value);
		ValueRet get_value(JsonValue::Array& array_value);
		ValueRet get_value(JsonValue::Object& object_value);

	private:
		JsonType type;
		
		double number_value;
		Str string_value;
		Array array_value;
		Object object_value;
	};

	struct JsonParser
	{
		static JsonValue::Ptr		parse(const char* json, ParseRet& ret);
		static JsonValue::Ptr		parse_value(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_null(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_true(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_false(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_number(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_string(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_array(JsonContext::Ptr c, ParseRet& ret);
		static JsonValue::Ptr		parse_object(JsonContext::Ptr c, ParseRet& ret);

		static const char*			parse_hex4(const char* p, unsigned* u);
		static void					encode_utf8(JsonContext::Ptr c, unsigned u, JsonValue::Str& tmpstr);

		static void					parse_whitespace(JsonContext::Ptr c);
		static void					next(JsonContext::Ptr c, char ch) { assert(*c->json == (ch)); c->json++; }
		static void					char2Str(const char* strin, JsonValue::Str& strout, int len);
	};
}

#endif
