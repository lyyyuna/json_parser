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
