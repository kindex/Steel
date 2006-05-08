#include "line.h"

bool isCross(const Line a, const Line b, float &t, float &s)
{
	// a.base + a.b*t = b.base + b.b*s
	// s = ((a.base-b.base)*a.b*)&(b.b*a.b) / (b.b x a.b)^2
	v3 bbxab = (b.a*a.a);
	double len2 = bbxab.getSquaredLengthd();
	if(len2<EPSILON2) return false; // ����� �����������
	
	s = ((a.base - b.base)*a.a)&bbxab / len2;

	v3 p = b.point(s);

	v3 p_abase = (p-a.base);
	v3 col = p_abase*a.a;
	if(col.getSquaredLengthd()>EPSILON2) return false; // ����� ����������� �� ����� �� ������ ������ - ������ �� �����������

	t = (p_abase & a.a) / (a.a & a.a);

	return true;
}

inline bool isCross(const Line a, const Line b)
{
	float t;
	float s;
	return isCross(a, b, t, s);
}
