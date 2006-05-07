#include "line.h"

bool isCross(const Line a, const Line b, float &t, float &s)
{
	// a.base + a.b*t = b.base + b.b*s
	// s = ((a.base-b.base)*a.b*)&(b.b*a.b) / (b.b x a.b)^2
	v3 bbxab = (b.a*a.a);
	float len2 = bbxab.getSquaredLength();
	if(len2<EPSILON) return false; // линии параллельны
	
	s = ((a.base - b.base)*a.a)&bbxab / len2;

	v3 p = b.point(s);

	v3 p_abase = (p-a.base);
	v3 col = p_abase*a.a;
	if(col.getSquaredLength()>EPSILON) return false; // точка пересечения не лежит на первой прямой - прямые не перескаются

	t = (p_abase & a.a) / (a.a & a.a);

	return true;
}

inline bool isCross(const Line a, const Line b)
{
	float t;
	float s;
	return isCross(a, b, t, s);
}
