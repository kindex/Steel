//////////////////////////////////////////////////////////////////////////////////////////
//	Maths.h
//	Include maths headers
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	17th December 2002	-	Removed QUATERNION
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef __MATH_MATH_H
#define __MATH_MATH_H

#include <math.h>



//VC++ math.h (and others) do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define G_PI (M_PI/180)

//An epsilon value for comparisons
#ifndef EPSILON
#define EPSILON 1.0e-6f
#endif

#ifndef EPSILON2
#define EPSILON2 1.0e-12
#endif


#ifndef INF
#define INF 1.0e+12f
#endif


template<typename T> T sqr(T x);
int sgn(float A);
//template<typename T>int sgn(T A);


//#include "PLANE.h"
//#include "COLOR.h"
#endif
