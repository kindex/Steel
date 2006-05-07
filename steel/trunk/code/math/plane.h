#ifndef __MATH_PLANE_H
#define __MATH_PLANE_H

#include "vector3d.h"
#include "line.h"

struct Plane // ѕлоскость: base + a*s + b*t
{
// base - точка остчЄта (нижний угол)
// a,b - напр€вл€ющие двух векторов этой плоскости

// ƒл€ треугольника
// 0<= t <=1
// 0<= s <=1
// 0<= t+s <=1

	v3 base, a, b;

	Plane() {}
	Plane(v3 _base, v3 _a, v3 _b): base(_base), a(_a), b(_b) {}
	v3 point(const float s, const float t) { return base + a*s + b*t; }
};

bool isCross(const Plane a, const Line b, float &s, float &t, float &k);
bool isCrossTrgLine(const Plane a, const Line b, float &k);


#endif
