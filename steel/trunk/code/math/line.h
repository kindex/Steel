// Line, LineSement 

#ifndef __MATH_LINE_H
#define __MATH_LINE_H

#include "vector3d.h"

struct Line3 // Line 3D
{// множество точек - отрезок, пр€ма€
// base + t*b
// 0<=t<=1 - дл€ отрезка
// t - любой - дл€ пр€мой
// base - точка остчЄта (нижний угол)
// b - напр€вл€ющaja
	v3	base, a;

	Line3() {}
	Line3(v3 _base, v3 _a): base(_base), a(_a) {}
	v3 point(const float s) const  { return base + a*s; }
};
typedef Line3 Line;

bool isCross(const Line a, const Line b, float &t, float &s);
inline bool isCross(const Line a, const Line b);



#endif
