#include <iostream>
#include <string>
#include "lyyjson.h"
#include "unittest.h"

using namespace lyy;

int main()
{
	return 0;
}

TEST_CASE(parse_true)
{
	JsonValue v;
	v.type = JsonType::FALSE;

	TEST_ASSERT(ParseRet::PARSE_OK == JsonParser::parse(v, "true"));
	TEST_ASSERT(JsonType::TRUE == v.type);
	// TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "tru"));
}

TEST_CASE(parse_false)
{
	JsonValue v;
	v.type = JsonType::TRUE;

	TEST_ASSERT(ParseRet::PARSE_OK == JsonParser::parse(v, "false"));
	TEST_ASSERT(JsonType::FALSE == v.type);
	// TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "fals"));
}

TEST_CASE(parse_null)
{
	JsonValue v;
	v.type = JsonType::FALSE;

	TEST_ASSERT(ParseRet::PARSE_OK == JsonParser::parse(v, "null"));
	TEST_ASSERT(JsonType::JNULL == v.type);
	// TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "nul"));
}

TEST_CASE(parse_expect_value)
{
	JsonValue v;
	v.type = JsonType::FALSE;

	TEST_ASSERT(ParseRet::PARSE_EXCEPT_VALUE == JsonParser::parse(v, ""));
	TEST_ASSERT(JsonType::JNULL == v.type);

	v.type = JsonType::FALSE;

	TEST_ASSERT(ParseRet::PARSE_EXCEPT_VALUE == JsonParser::parse(v, " "));
	TEST_ASSERT(JsonType::JNULL == v.type);
}

TEST_CASE(parse_invalid_value)
{
	JsonValue v;
	v.type = JsonType::FALSE;

	TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "nul"));
	TEST_ASSERT(JsonType::JNULL == v.type);

	TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "%^"));
	TEST_ASSERT(JsonType::JNULL == v.type);
}

TEST_CASE(parse_root_not_singular)
{
	JsonValue v;
	v.type = JsonType::FALSE;

	TEST_ASSERT(ParseRet::PARSE_ROOT_NOT_SINGULAR == JsonParser::parse(v, "null false"));
	TEST_ASSERT(JsonType::JNULL == v.type);
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

TEST_CASE(parse_number_too_big)
{
	TEST_ERROR(ParseRet::PARSE_NUMBER_TOO_BIG, "1e309");
	TEST_ERROR(ParseRet::PARSE_NUMBER_TOO_BIG, "-1e309");
}
