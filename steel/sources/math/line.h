/*id*********************************************************
	File: math/line.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
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

struct Line // Line 3D
{	v3	base, a;

	Line() {}
	Line(v3 _base, v3 _a): base(_base), a(_a) {}
	v3 point(const float s) const  { return base + a*s; }
};

// пересечение двух линий
bool isCross(const Line a, const Line b, float &t, float &s);
// мы уверены, что прямые пересекаются. 
bool isCrossFast(const Line a, const Line b, float &s);

inline bool isCross(const Line a, const Line b)
{
	float t;
	float s;
	return isCross(a, b, t, s);
}

// distance from line to point
v3 getPointLineVector(const v3& point, const Line& line);
float getPointLineDist(const v3& point, const Line& line);

// distance from line segment to point
v3 getPointLinesegmentVector(const v3& point, const Line& line);
float getPointLinesegmentDist(const v3& point, const Line& line);

#endif
