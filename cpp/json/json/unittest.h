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
		ParseRet ret;													\
		auto v = JsonParser::parse(json, ret);							\
		TEST_ASSERT(ParseRet::PARSE_OK == ret);							\
		TEST_ASSERT(JsonType::JNUMBER == v->get_type());				\
		double number;													\
		TEST_ASSERT(ValueRet::OK == v->get_value(number));				\
		TEST_ASSERT(expect == number);									\
						} while(0)

#define TEST_ERROR(error, json)											\
	do {																\
		ParseRet ret;													\
		auto v = JsonParser::parse(json, ret);							\
		TEST_ASSERT(error == ret);										\
		TEST_ASSERT(JsonType::JNULL == v->get_type());					\
						} while(0)

#define TEST_ERROR_SINGULAR(error, json)								\
	do {																\
		ParseRet ret;													\
		auto v = JsonParser::parse(json, ret);							\
		TEST_ASSERT(error == ret);										\
						} while(0)


#define TEST_STRING( expect,   json)									\
	do{																	\
		ParseRet ret;													\
		auto v = JsonParser::parse(json, ret);							\
		TEST_ASSERT(ParseRet::PARSE_OK == ret);							\
		TEST_ASSERT(JsonType::JSTRING == v->get_type());				\
		JsonValue::Str str;												\
		TEST_ASSERT(ValueRet::OK == v->get_value(str));					\
		auto expectsize = sizeof(expect) - 1;							\
		TEST_ASSERT(expectsize == str.size());							\
		for (JsonValue::Str::size_type i = 0; i < str.size(); i++)		\
			TEST_ASSERT((unsigned char)expect[i] == str[i]);			\
	} while(0)


#endif