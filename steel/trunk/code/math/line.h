/*id*********************************************************
    Unit: math/line
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	Licence:
        Только для Division
    Description:
		Line, LineSement
************************************************************/

#ifndef __MATH_LINE_H
#define __MATH_LINE_H

#include "vector3d.h"

/* множество точек - отрезок, прямая
 base + t*b
 0<=t<=1 - для отрезка
 t - любой - для прямой
 base - точка остчёта (нижний угол)
 b - напрявляющaя */

struct Line3 // Line 3D
{	v3	base, a;

	Line3() {}
	Line3(v3 _base, v3 _a): base(_base), a(_a) {}
	v3 point(const float s) const  { return base + a*s; }
};

typedef Line3 Line;

// пересечение двух линий
bool isCross(const Line a, const Line b, float &t, float &s);
// мы уверены, что прямые пересекаются. 
bool isCrossFast(const Line a, const Line b, float &s);

inline bool isCross(const Line a, const Line b);

#endif
