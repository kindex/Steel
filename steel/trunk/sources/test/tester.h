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
 ************************************************************/

#ifndef TESTER_H
#define TESTER_H

#include "../steel.h"

#include "../common/steel_vector.h"
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

#define CHECK_NOT_NULL(value, message) \
{ \
	if((value) == NULL)	\
	{	\
		Error(std::string(#value) + " is NULL. " + (message));\
	}	\
}

#define CHECK_EQUAL(a, b, message) \
{ \
	if((a) != (b))  \
	{	\
		Error(std::string(#a) + " != " + #b + ". " +  (message));\
	}	\
}

struct TestInfo
{
	int level;
	int errorCount;
	svector<std::string> errors;
	
	TestInfo(void): level(0), errorCount(0) {}
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
