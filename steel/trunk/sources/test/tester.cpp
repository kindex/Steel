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
		�������� ������� � ��������� ��� ���������������� ����.
 ************************************************************/
#include "../steel.h"
#include "tester.h"
#include "../common/utils.h"
#include "../common/logger.h"

#include <iostream>

using namespace std;

TestInfo testInfo;

void TestInfo::report()
{
	string message = IntToStr(errors.size()) + " errors";
	cout << "Test report: " << message << endl;
	log_msg("test", message);
}

// ********************** Tester *********************

void Tester::TestAll(void)
{
	int errors = testInfo.errorCount;
	TestMembers();
	Test();
	std::cout << std::string(testInfo.level-1, '\t');
	int newErrors = testInfo.errorCount - errors;
	if(newErrors == 0) 
		std::cout << "Ok" << std::endl;
	else
		std::cout << newErrors << " Errors" << std::endl;
}


void Tester::Error(std::string message)
{
	testInfo.errorCount++;
	testInfo.push(message);
	std::cout << std::string(testInfo.level, '\t') << "Error: " << message << std::endl;
}
