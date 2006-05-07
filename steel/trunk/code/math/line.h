// Line, LineSement 

#ifndef __MATH_LINE_H
#define __MATH_LINE_H

#include "vector3d.h"

struct Line3 // Line 3D
{// ��������� ����� - �������, ������
// base + t*b
// 0<=t<=1 - ��� �������
// t - ����� - ��� ������
// base - ����� ������� (������ ����)
// b - ����������aja
	v3	base, a;

	Line3() {}
	Line3(v3 _base, v3 _a): base(_base), a(_a) {}
	v3 point(const float s) const  { return base + a*s; }
};
typedef Line3 Line;

bool isCross(const Line a, const Line b, float &t, float &s);
inline bool isCross(const Line a, const Line b);



#endif
