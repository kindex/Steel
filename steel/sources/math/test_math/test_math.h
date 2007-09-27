/*id*********************************************************
	File: math/test_math/test_math.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		math Selftest
 ************************************************************/

#ifndef MATH_TEST_H
#define MATH_TEST_H

#include "../../steel.h"

#if TESTING == 1

#include "../../test/tester.h"

class MathTest: public Tester
{
public:
	void Test(void)
	{
		TestPlanes();
	}
	void TestPlanes();
};

#endif
#endif

