/*id*********************************************************
	File: math/line.cpp
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

#include "line.h"

bool isCross(const Line a, const Line b, float &t, float &s)
{
	// a.base + a.b*t = b.base + b.b*s
	// s = ((a.base-b.base)*a.b*)&(b.b*a.b) / (b.b x a.b)^2
	v3 bbxab = (b.a*a.a);
	double len2 = bbxab.getSquaredLengthd();
	if(len2<EPSILON2) return false; // линии параллельны
	
	s = (((a.base - b.base)*a.a)&bbxab / (float)len2);

	v3 p = b.point(s);

	v3 p_abase = (p-a.base);
	v3 col = p_abase*a.a;
	if(col.getSquaredLengthd()>EPSILON2) return false; // точка пересечения не лежит на первой прямой - прямые не перескаются

	t = (p_abase & a.a) / (a.a & a.a);

	return true;
}
// мы уверены, что прямые пересекаются
bool isCrossFast(const Line a, const Line b, float &s)
{
	// a.base + a.b*t = b.base + b.b*s
	// s = ((a.base-b.base)*a.b*)&(b.b*a.b) / (b.b x a.b)^2
	v3 bbxab = (b.a*a.a);
	double len2 = bbxab.getSquaredLengthd();
	
	s = ((a.base - b.base)*a.a)&bbxab / (float)len2;

	return true;
}

// distance from line to point
float getPointLineDist(const v3& point, const Line& line)
{
    return (line.base - point).crossProduct(line.base + line.a - point).getLength()/(line.a.getLength());
}
// distance from line segment to point
float getPointLinesegmentDist(const v3& point, const Line& line)
{
    float d1 = (line.base - point).getSquaredLength();
    float d2 = (line.base + line.a - point).getSquaredLength();
    float d3 = line.a.getSquaredLength();

    if (d2 + d3 < d1)
    {
        return sqrt(d2);
    }
    if (d1 + d3 < d2)
    {
        return sqrt(d1);
    }
    return getPointLineDist(point, line);
}

// distance from line to point
v3 getPointLineVector(const v3& point, const Line& line)
{
    v3 normal = (line.base - point).crossProduct(line.a);
    float h = normal.getLength()/(line.a.getLength());
    return line.a.crossProduct(normal).getNormalized()*h;
}
// distance from line segment to point
v3 getPointLinesegmentVector(const v3& point, const Line& line)
{
    float d1 = (line.base - point).getSquaredLength();
    float d2 = (line.base + line.a - point).getSquaredLength();
    float d3 = line.a.getSquaredLength();

    if (d2 + d3 < d1)
    {
        return line.base + line.a - point;
    }
    if (d1 + d3 < d2)
    {
        return line.base - point;
    }
    return getPointLineVector(point, line);
}
