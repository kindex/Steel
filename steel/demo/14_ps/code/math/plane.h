/*id*********************************************************
    Unit: math/plane
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
        Steel Engine License
    Description:
		Plane, Triangle
************************************************************/

#ifndef __MATH_PLANE_H
#define __MATH_PLANE_H

#include "vector3d.h"
#include "line.h"

struct Plane;

// ���������� �� ��������� ������
bool isCross(const Plane a, const Line b, float &k);

// ���������� �� ����������� ������
bool isCrossTrgLine(const Plane a, const Line b, float &k);

// ��������� �� ����� ����� ������ (base + a*t) � (base + b*t)
bool isBetween(const v3 point, const v3 base, const v3 a, const v3 b);

// ��������� �� ������ ������ � ���������� a � ������� d
// ����� p
bool pointInPrism(const Plane a, const v3 d, v3 p);

// ���������� �� ���������� ����� (a:direction) ������� (b.base + b.a*k)
bool crossMLineLine(const Line a, const v3 direction, const Line b, float &k);

// ��������� �� ����� �� ������ ������� �� ���������
bool byRightSide(const v3 point, const Plane a);

/* ���������: base + a*s + b*t
 base - ����� ������� (������ ����)
 a,b - ������������ ���� �������� ���� ���������

 ��� ������������
 0<= t <=1
 0<= s <=1
 0<= t+s <=1
*/
struct Plane 
{
	v3 base, a, b;

	Plane() {}
	Plane(v3 _base, v3 _a, v3 _b): base(_base), a(_a), b(_b) {}
	v3 point(const float s, const float t) const { return base + a*s + b*t; }

	bool isInTrg(v3 point) const;
	inline bool crossLine(const Line b, float &k) const 
	{ 
		return isCrossTrgLine(*this, b, k); 
	}
};


#endif
