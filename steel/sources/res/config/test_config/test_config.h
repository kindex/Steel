/*id*********************************************************
	File: res/config/test_config/test_config.h
	Unit: Config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Config Selftest
 ************************************************************/

#ifndef __RES__CONFIG_TEST_H
#define __RES__CONFIG_TEST_H

#include "../../../steel.h"
#include "../../../test/tester.h"
#include "../config.h"
#include "../config_parser.h"

class ConfigTest: public Tester
{
public:
	void Test(void)
	{
		TestFind();
	}
	void TestFind();
};

class ConfigParserTest: public Tester
{
public:
	void Test(void)
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

