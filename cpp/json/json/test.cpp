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
	TEST_ASSERT(ParseRet::PARSE_INVALID_VALUE == JsonParser::parse(v, "tru"));
}
