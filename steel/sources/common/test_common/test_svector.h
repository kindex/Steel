/*id*********************************************************
	File: common/test_common/test_svector.h
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
#ifndef __COMMOON__STEEL_VECTOR_TEST_H
#define __COMMOON__STEEL_VECTOR_TEST_H

#include "../../steel.h"

#if TESTING == 1

#include "../../test/tester.h"
#include "../svector.h"

class svectorTest: public Tester
{
public:
	void Test(void);

private:
	void TestMemory(void);
};

#endif
#endif
