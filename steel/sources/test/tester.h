/*id*********************************************************
	File: tester.h
	Unit: tester
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Основные макросы и процедуры для самотестирования кода.
	TODO:
		Check test execution time and memory leak
 ************************************************************/

#ifndef TESTER_H
#define TESTER_H

#include "../steel.h"

#include "../common/svector.h"
#include "../common/logger.h"
#include <string>
#include <iostream>

#define TEST(C) \
{	\
	std::cout << std::string(testInfo.level, '\t') << "Testing " # C << std::endl; \
	testInfo.level++; \
	C##Test testClass;	\
	testClass.TestAll();	\
	testInfo.level--; \
}

#define TEST_CODE(code) \
{\
	testInfo.testCount++;\
	code;\
}

#define TEST_ERROR(message) \
{ \
	Error(message);\
}

#define CHECK_NOT_NULL(value, message) \
{ \
	if((value) == NULL)	\
	{	\
		Error(std::string(#value) + " is NULL. " + (message));\
	}	\
}

#define CHECK_TRUE(expr, message) \
{ \
	if(!(expr))  \
	{	\
		Error(std::string("Failed ") + #expr + ". " +  (message));\
	}	\
}

struct TestInfo
{
	int testCount;
	int level;
	int errorCount;
	svector<std::string> errors;
	
	TestInfo(void): level(0), errorCount(0), testCount(0) {}
	void push(std::string newError) { errors.push_back(newError); }
	void report(void);
};

extern TestInfo testInfo;

class Tester
{
public:
	virtual void TestAll(void);
	virtual void Test(void){}
	virtual void TestMembers(void) { }
	virtual void Error(std::string message);
};

#endif
