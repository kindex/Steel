/*id*********************************************************
	File: math/test_math/test_math.cpp
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
#include "../../steel.h"

#if TESTING == 1

#include "test_math.h"
#include "../plane.h"
#include "../matrix34.h"

void MathTest::TestPlanes()
{
     CHECK_EQUAL(getPointLineDist(v3(0, 0, 0), Line(v3(0, 0, 0), v3(1, 0, -1))), 0.0f, "Point in line");
     CHECK_EQUAL(getPointLineDist(v3(0, 0, 0), Line(v3(10.0f, 0, 0), v3(0, 0, 1))), 10.0f, "Point to line dist");
     CHECK_EQUAL(getPointLineDist(v3(0, 0, 0), Line(v3(1, 0, 0), v3(-1, 1, 0))), sqrt(2.0f)/2.0f, "Point to line dist");
     CHECK_EQUAL(getPointLineDist(v3(0.5f, 0, 0), Line(v3(1, 0, 0), v3(1, 1, 0))), sqrt(2.0f)/4.0f, "Point to line dist");

     CHECK_EQUAL(getPointLinesegmentDist(v3(0, 0, 0), Line(v3(0, 0, 0), v3(1, 0, -1))), 0.0f, "Point in line segment");
     CHECK_EQUAL(getPointLinesegmentDist(v3(0, 0, 0), Line(v3(7.0f, 0, 0), v3(0, 0, -1))), 7.0f, "Point to line segment ");
     CHECK_EQUAL(getPointLinesegmentDist(v3(0, 0, 0), Line(v3(7.0f, 0, 2), v3(0, 0, -1))), sqrt(sqr(7.0f) + sqr(1.0f)), "Point to line segment ");
    
     CHECK_EQUAL(getPointPlaneDist(v3(0,0,0), Plane(v3(0,0,0), v3(1,0,0), v3(0,1,0))), 0.0f, "Point in plane");
     CHECK_EQUAL(getPointPlaneDist(v3(0,0,5.7f), Plane(v3(0,0,0), v3(1,0,0), v3(0,1,0))), 5.7f, "Point to plane dist");
     CHECK_EQUAL(getPointPlaneDist(v3(0, 0, 0), Plane(v3(0, 0, 1), v3(1, 0, -1), v3(0,1, -1))), sqrt(1.0f/3.0f), "Point to plane dist");

     CHECK_EQUAL(getPointTrgDist(v3(0, 0, 0), Plane(v3(1, 1, 1), v3(1, 0, 0), v3(0, 1, 0))), sqrt(3.0f), "Point to plane dist");
     CHECK_EQUAL(getPointTrgDist(v3(0, 0, 0), Plane(v3(0, 0, 15.0f), v3(1, 0, 0), v3(0, 1, 0))), 15.0f, "Point to plane dist");
     CHECK_EQUAL(getPointTrgDist(v3(0, 0, 0), Plane(v3(-10, 15, 17.0f), v3(60, 0, 0), v3(0, 0, 90))), sqrt(sqr(15.0f) + sqr(17.0f)), "Point to plane dist");

     CHECK_EQUAL(getPointLineVector(v3(0, 0, 0), Line(v3(0, 0, 0), v3(1, 0, -1))), v3(0, 0, 0), "Point in line");
     CHECK_EQUAL(getPointLineVector(v3(0, 0, 0), Line(v3(10.0f, 0, 0), v3(0, 0, 1))), v3(10.0f, 0, 0), "Point to line dist");
     CHECK_EQUAL(getPointLineVector(v3(0, 0, 0), Line(v3(1, 0, 0), v3(-1, 1, 0))), v3(0.5f, 0.5f, 0), "Point to line dist");
     CHECK_EQUAL(getPointLineVector(v3(0.5f, 0, 0), Line(v3(1, 0, 0), v3(1, 1, 0))), v3(0.25f, -0.25f, 0), "Point to line dist");

     CHECK_EQUAL(getPointLinesegmentVector(v3(0, 0, 0), Line(v3(0, 0, 0), v3(1, 0, -1))), v3(0, 0, 0), "Point in line segment");
     CHECK_EQUAL(getPointLinesegmentVector(v3(0, 0, 0), Line(v3(7.0f, 0, 0), v3(0, 0, -1))), v3(7.0f, 0, 0), "Point to line segment ");
     CHECK_EQUAL(getPointLinesegmentVector(v3(0, 0, 0), Line(v3(7.0f, 0, 2), v3(0, 0, -1))), v3(7.0f, 0, 1.0f), "Point to line segment ");
    
     CHECK_EQUAL(getPointTrgVector(v3(0, 0, 0), Plane(v3(1, 1, 1), v3(1, 0, 0), v3(0, 1, 0))), v3(1, 1, 1), "Point to plane dist");
     CHECK_EQUAL(getPointTrgVector(v3(0, 0, 0), Plane(v3(0, 0, 15.0f), v3(1, 0, 0), v3(0, 1, 0))), v3(0, 0, 15.0f), "Point to plane dist");
     CHECK_EQUAL(getPointTrgVector(v3(0, 0, 0), Plane(v3(-10, 15, 17.0f), v3(60, 0, 0), v3(0, 0, 90))), v3(0, 15.0f, 17.0f), "Point to plane dist");
}

void MathTest::TestMatrix34()
{
	matrix34 m;
	v3 translation(1, 2, 3);
	m.loadIdentity();
	m.setTranslation(translation);

	v3 newtranslation = m.getTranslation();

	CHECK_EQUAL(translation, newtranslation, "matrix43::getTranslation()");

	v3 zero;
	zero.loadZero();

	v3 newtranslation2 = zero * m;

	CHECK_EQUAL(translation, newtranslation2, "matrix43::operator*()");
}

#endif
