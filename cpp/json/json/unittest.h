#ifndef LYY_JSON_UNIT_TEST_H
#define LYY_JSON_UNIT_TEST_H

#include <iostream>
#include <string>

using namespace std;

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

#endif