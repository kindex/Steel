/*id*********************************************************
	File: res/config/test_config/test_config.h
	Unit: Config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Config Selftest
 ************************************************************/

#ifndef __RES__CONFIG_TEST_H
#define __RES__CONFIG_TEST_H

#include "../../../steel.h"

#if TESTING == 1

#include "../../../test/tester.h"
#include "../config.h"
#include "../config_parser.h"

class ConfigTest: public Tester
{
public:
	void Test()
	{
		TestFind();
        TestSerialization();
	}
	void TestFind();
	void TestSerialization();
};

class ConfigParserTest: public Tester
{
public:
	void Test()
	{
		TestParser();
	}
	void TestParser();
};


class ResCollectionConfigTest: public Tester
{
public:

};

#endif
#endif

