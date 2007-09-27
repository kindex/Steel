/*id*********************************************************
	File: math/plane.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Plane, Triangle
************************************************************/

#include "plane.h"
#include <xutility>

/*
Находится ли точка между лучами (base + a*t) и (base + b*s)
*/
bool isBetween(v3 point, const v3 base, const v3 a, const v3 b)
{
	point -= base;
	v3 ab = a.crossProduct(b);
	float k1 =(ab)&(a*point);
	float k2 =(ab)&(point*b);
	return k1 >- EPSILON2 && k2 >- EPSILON2;
}

bool isCross(const Plane& a, const Line b, float &k)
{
// a.base + a.a*s + a.b*t = b.base + b.a * k

// nahodim k

	v3 a_b = (a.base-b.base);
	v3 abxaa = a.b*a.a;
	if (abxaa.getSquaredLengthd()<EPSILON2)
	{
		return false;
	}
	v3 baxaaxabxaa =  (b.a * a.a) * abxaa;
	double len2 = baxaaxabxaa.getSquaredLengthd();
	if (len2 < EPSILON2)
	{
		return false;
	}

	k = (((a_b*a.a)*(abxaa)) & baxaaxabxaa) / (float)len2;

	return true;
}

// пересекает ли прямая (b.base + b.a*k) тругольник
// если да, то возвращается k
bool isCrossTrgLine(const Plane a, const Line b, float &k)
{
	if (!isCross(a, b, k))
	{
		return false;
	}

	v3 p = b.point(k);

	return a.isInTrg(p);
}


bool Plane::isInTrg(v3 p) const
{
	return isBetween(p, base, a, b) && isBetween(p, base + a, b - a, -a);
}

bool byRightSide(const v3 point, const Plane a)
{
	return ((a.a*a.b)&(point-a.base))>EPSILON;
}

bool pointInPrism(const Plane a, const v3 d, v3 p)
{
	p -= a.base;
	
	bool c1 = ((a.a*a.b)&p)>=EPSILON;
	bool c2 = ((  d*a.a)&p)>=EPSILON;
	bool c3 = ((a.b*  d)&p)>=EPSILON; 
	bool c4 = ((d*(a.b-a.a))&(p-a.a))>=EPSILON;
	bool c5 = ((a.b*a.a)&(p-d))>=EPSILON;

	return c1 && c2 && c3 && c4 && c5;
}

// Пересекает ли движущаяся линия (a:direction) отрезок (b.base + b.a*k)
bool crossMLineLine(const Line a, const v3 direction, const Line b, float &k)
{ 
	Plane p(a.base, a.a, direction);

	if (!isCross(p, b, k))
	{
		return false;
	}
	if (k<-EPSILON || k>1+ EPSILON)
	{
		return false;
	}

	v3 point = b.point(k);

	return  isBetween(point, a.base, direction, a.a) && 
			isBetween(point, a.base + a.a + direction, -a.a, -direction);
}

v3 Plane::reflect(const v3 vector) const
{
    v3 an = a.getNormalized();
    v3 bn = b.getNormalized();
    v3 mid = an*vector.dotProduct(an) + bn*vector.dotProduct(bn);
    v3 result =  2*mid - vector;
    return result;
}

float getPointTrgDist(const v3& point, const Plane& triangle)
{
    float k;
    if (isCrossTrgLine(triangle, Line(point, triangle.getNormal().getNormalized()), k))
    {
        return abs(k);
    }
    else
    {
        float l1 = getPointLinesegmentDist(point, Line(triangle.base, triangle.a));
        float l2 = getPointLinesegmentDist(point, Line(triangle.base, triangle.b));
        float l3 = getPointLinesegmentDist(point, Line(triangle.base + triangle.b, triangle.a));
        return std::min(l1, std::min(l2, l3));
    }
}

float getPointPlaneDist(const v3& point, const Plane& plane)
{
    float k;
    isCross(plane, Line(point, plane.getNormal().getNormalized()), k);
    return abs(k);
}

v3 getPointTrgVector(const v3& point, const Plane& triangle)
{
    float k;
    if (isCrossTrgLine(triangle, Line(point, triangle.getNormal().getNormalized()), k))
    {
        return triangle.getNormal().getNormalized()*k;
    }
    else
    {
        v3 l1 = getPointLinesegmentVector(point, Line(triangle.base, triangle.a));
        v3 l2 = getPointLinesegmentVector(point, Line(triangle.base, triangle.b));
        v3 l3 = getPointLinesegmentVector(point, Line(triangle.base + triangle.b, triangle.a));
        float d1 = l1.getSquaredLength();
        float d2 = l2.getSquaredLength();
        float d3 = l3.getSquaredLength();
        if (d1 < d2 && d1 < d3)
        {
            return l1;
        }
        else if (d2 < d3)
        {
            return l2;
        }
        else
        {
            return l3;
        }
    }
}
