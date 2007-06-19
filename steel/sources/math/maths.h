/*id*********************************************************
	File: math/maths.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Полезные математические функции функции
 *****************************************************************/

#ifndef __MATH_MATH_H
#define __MATH_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

//VC++ math.h (and others) do not define M_PI
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define G_PI (M_PI/180)

const float EPSILON = 1.0e-6f;
const double EPSILON2 = 1.0e-10;
const float INF = 1.0e+12f;

float clamp(float a);

template<typename T> T sqr(T x) 
{
	return x*x;
}

template<typename T> int sgn(T A) 
{ 
	return (A>EPSILON)-(A<-EPSILON);
}

#endif
