#ifndef LYY_JSON_UNIT_TEST_H
#define LYY_JSON_UNIT_TEST_H

#include <iostream>
#include <string>
#include "lyyjson.h"

using namespace std;
using namespace lyy;

#ifndef LYY_JSON_RELEASE

#define TEST_CASE(NAME)								\
	extern void TEST_CASE_FUNCTION_##NAME(void);	\
	class TEST_CASE_CLASS_##NAME					\
	{												\
	public:											\
		TEST_CASE_CLASS_##NAME()					\
		{											\
			cout << "test for: ";					\
			cout << #NAME;							\
			TEST_CASE_FUNCTION_##NAME();			\
			cout << " . test ok. " << endl;			\
		}											\
	} TEST_CASE_INSTANCE_##NAME;					\
	void TEST_CASE_FUNCTION_##NAME(void)

#else

#define TEST_CASE(NAME)								\
void TEST_CASE_FUNCTION_##NAME(void)

#endif


#define TEST_ASSERT(CONDITION) do{ if (!(CONDITION)) throw 0; } while (0)
#define TEST_PRINT(MESSAGE) cout << (MESSAGE) << endl;

#define TEST_NUMBER(expect, json)										\
	do {																\
		JsonValue v;													\
		TEST_ASSERT(ParseRet::PARSE_OK == JsonParser::parse(v, json));	\
		TEST_ASSERT(JsonType::NUMBER == v.type);						\
		TEST_ASSERT(expect == v.num);									\
			} while(0)

#define TEST_ERROR(error, json)									\
	do {														\
		JsonValue v;											\
		TEST_ASSERT(error == JsonParser::parse(v, json));		\
		TEST_ASSERT(JsonType::JNULL == v.type);					\
	} while (0)

#endif