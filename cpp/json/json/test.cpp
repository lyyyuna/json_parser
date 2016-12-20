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

