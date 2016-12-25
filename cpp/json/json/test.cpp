#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "lyyjson.h"
#include "unittest.h"

using namespace lyy;

int main()
{
	return 0;
}

TEST_CASE(parse_null)
{
	ParseRet ret;

	auto v = JsonParser::parse("null", ret);
	TEST_ASSERT(ParseRet::PARSE_OK == ret);
	TEST_ASSERT(JsonType::JNULL == v->get_type());
	// TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "nul"));
}

TEST_CASE(parse_true)
{
	ParseRet ret;

	auto v = JsonParser::parse("true", ret);

	TEST_ASSERT(ParseRet::PARSE_OK == ret);
	TEST_ASSERT(JsonType::JTRUE == v->get_type());
	// TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "tru"));
}

TEST_CASE(parse_false)
{
	ParseRet ret;

	auto v = JsonParser::parse("false", ret);

	TEST_ASSERT(ParseRet::PARSE_OK == ret);
	TEST_ASSERT(JsonType::JFALSE == v->get_type());
	// TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "tru"));
}

TEST_CASE(parse_number)
{
	TEST_NUMBER(0.0, "0");
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "-1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(1e10, "1e10");
	TEST_NUMBER(1E+10, "1E+10");
	TEST_NUMBER(1E-10, "1E-10");
	TEST_NUMBER(-1E10, "-1E10");
	TEST_NUMBER(-1e10, "-1e10");
	TEST_NUMBER(-1E+10, "-1E+10");
	TEST_NUMBER(-1E-10, "-1E-10");
	TEST_NUMBER(1.234E+10, "1.234E+10");
	TEST_NUMBER(1.234E-10, "1.234E-10");
	TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

	TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
	TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
	TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
	TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
	TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
	TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

TEST_CASE(parse_number_FET)
{
	/* invalid number */
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "+0");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "+1");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "INF");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "inf");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "NAN");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "nan");
}

TEST_CASE(parse_number_too_big_FET)
{
	TEST_ERROR(ParseRet::PARSE_NUMBER_TOO_BIG, "1e309");
	TEST_ERROR(ParseRet::PARSE_NUMBER_TOO_BIG, "-1e309");
}

TEST_CASE(parse_invalid_value_FET)
{
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "nul");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "?");
}

TEST_CASE(parse_root_not_singular_FET)
{
	TEST_ERROR(ParseRet::PARSE_ROOT_NOT_SINGULAR, "null x");

	// actually invalid number
	TEST_ERROR_SINGULAR(ParseRet::PARSE_ROOT_NOT_SINGULAR, "0123");
	TEST_ERROR_SINGULAR(ParseRet::PARSE_ROOT_NOT_SINGULAR, "0x0");
	TEST_ERROR_SINGULAR(ParseRet::PARSE_ROOT_NOT_SINGULAR, "0x123");
}

TEST_CASE(parse_expect_value_FET)
{
	TEST_ERROR(ParseRet::PARSE_EXCEPT_VALUE, "");
	TEST_ERROR(ParseRet::PARSE_EXCEPT_VALUE, " ");
}

TEST_CASE(parse_string)
{
	TEST_STRING("", "\"\"");
	TEST_STRING("Hello", "\"Hello\"");
	TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
	TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
}

TEST_CASE(parse_missing_quotation_mark_FET)
{
	TEST_ERROR(ParseRet::PARSE_MISS_QUOTATION_MARK, "\"");
	TEST_ERROR(ParseRet::PARSE_MISS_QUOTATION_MARK, "\"abc");
}

TEST_CASE(parse_invalid_string_escape_FET)
{
	TEST_ERROR(ParseRet::PARSE_INVALID_STRING_ESCAPE, "\"\\v\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

#if 0
TEST_CASE(parse_invalid_string_char_FET)
{
	//TEST_ERROR(ParseRet::PARSE_INVALID_STRING_CHAR, "\"\x01\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
}
#endif


TEST_CASE(parse_unicode_string)
{
	TEST_STRING("Hello\0World", "\"Hello\\u0000World\"");
	TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
	TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
	TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
	TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
	TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
}

TEST_CASE(parse_invalid_unicode_surrogate_FET)
{
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_SURROGATE, "\"\\uDBFF\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\\\\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uDBFF\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uE000\"");
}

TEST_CASE(parse_invalid_unicode_hex_FET)
{
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u0\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u01\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u012\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u/000\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\uG000\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u0/00\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u0G00\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u0/00\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u00G0\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u000/\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u000G\"");
	TEST_ERROR(ParseRet::PARSE_INVALID_UNICODE_HEX, "\"\\u 123\"");
}

TEST_CASE(parse_array)
{
	ParseRet ret;

	/**/
	auto v = JsonParser::parse("[ ]", ret);
	TEST_ASSERT(ParseRet::PARSE_OK == ret);
	TEST_ASSERT(JsonType::JARRAY == v->get_type());
	JsonValue::Array arr;
	ValueRet vret;
	vret = v->get_value(arr);
	TEST_ASSERT(ValueRet::OK == vret);
	TEST_ASSERT(0 == arr.size());

	/**/
	auto v2 = JsonParser::parse("[ null , false , true , 123 , \"abc\" ]", ret);
	TEST_ASSERT(ParseRet::PARSE_OK == ret);
	TEST_ASSERT(JsonType::JARRAY == v2->get_type());
	JsonValue::Array arr2;
	ValueRet vret2;
	vret2 = v2->get_value(arr2);
	TEST_ASSERT(ValueRet::OK == vret2);
	TEST_ASSERT(5 == arr2.size());

	auto v2_1 = arr2[0];
	TEST_ASSERT(JsonType::JNULL == v2_1->get_type());
	auto v2_2 = arr2[1];
	TEST_ASSERT(JsonType::JFALSE == v2_2->get_type());
	auto v2_3 = arr2[2];
	TEST_ASSERT(JsonType::JTRUE == v2_3->get_type());
	auto v2_4 = arr2[3];
	TEST_ASSERT(JsonType::JNUMBER == v2_4->get_type());
	double v2_4_number;
	v2_4->get_value(v2_4_number);
	TEST_ASSERT(123.0 == v2_4_number);
	auto v2_5 = arr2[4];
	TEST_ASSERT(JsonType::JSTRING == v2_5->get_type());
	JsonValue::Str v2_5_string;
	v2_5->get_value(v2_5_string);
	TEST_ASSERT(3 == v2_5_string.size());
	TEST_ASSERT('a' == v2_5_string[0]);
	TEST_ASSERT('b' == v2_5_string[1]);
	TEST_ASSERT('c' == v2_5_string[2]);

	/**/
	auto v3 = JsonParser::parse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]", ret);
	TEST_ASSERT(ParseRet::PARSE_OK == ret);
	TEST_ASSERT(JsonType::JARRAY == v3->get_type());
	JsonValue::Array arr3;
	ValueRet vret3;
	vret3 = v3->get_value(arr3);
	TEST_ASSERT(ValueRet::OK == vret3);
	TEST_ASSERT(4 == arr3.size());

	for (int i = 0; i < 4; ++i)
	{
		JsonValue::Ptr v3_1 = arr3[i];
		JsonValue::Array arr3_1;
		v3_1->get_value(arr3_1);
		TEST_ASSERT(i == arr3_1.size());
		for (int j = 0; j < i; ++j)
		{
			auto v3_e = arr3_1[j];
			TEST_ASSERT(JsonType::JNUMBER == v3_e->get_type());
			double v3_number;
			v3_e->get_value(v3_number);
			TEST_ASSERT((double)j == v3_number);
		}
	}
}

TEST_CASE(parse_invalid_array_FET)
{
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "[1,]");
	TEST_ERROR(ParseRet::PARSE_INVALID_VALUE, "[\"a\", nul]");
}

TEST_CASE(parse_miss_comma_or_square_bracket_FET)
{
	TEST_ERROR(ParseRet::PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1");
	TEST_ERROR(ParseRet::PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1}");
	TEST_ERROR(ParseRet::PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1 2");
	TEST_ERROR(ParseRet::PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[[]");
}
