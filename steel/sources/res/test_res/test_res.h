/*id*********************************************************
	File: res/test_res/test_res.h
	Unit: Res
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Res Selftest
 ************************************************************/
#ifndef __RES_TEST_H
#define __RES_TEST_H

#include "../../steel.h"

#if TESTING == 1

#include "../../test/tester.h"
#include "../res.h"
#include "../config/test_config/test_config.h"


class ResTest: public Tester
{
public:
	void TestMembers(void)
	{
		TEST(Config);
		TEST(ConfigParser);
		TEST(ResCollectionConfig);
	}
};


#endif
#endif
