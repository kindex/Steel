/*id*********************************************************
	File: common/test_common/test_svector.cpp
	Unit: Common
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Steel Vector Selftest
 ************************************************************/

#include "test_svector.h"

void svectorTest::Test(void)
{
	TestMemory();
}

void svectorTest::TestMemory(void)
{
	svector<int> a(10000000);
}
