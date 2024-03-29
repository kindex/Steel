/*id*********************************************************
	File: common/test_common/test_svector.cpp
	Unit: Common
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Steel Vector Selftest
 ************************************************************/

#include "../../steel.h"
#if TESTING == 1

#include "test_svector.h"
#include "../../common/types.h"

void svectorTest::Test(void)
{
	TestMemory();
}

void svectorTest::TestMemory(void)
{
	IntVector a(10000000);
}

#endif
