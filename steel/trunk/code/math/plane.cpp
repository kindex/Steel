#include "plane.h"

bool isCross(const Plane a, const Line b, float &s, float &t, float &k)
{
// a.base + a.a*s + a.b*t = b.base + b.a * k

// nahodim k

	v3 a_b = (a.base-b.base);
	v3 abxaa = a.b*a.a; 
	v3 baxaaxabxaa =  (b.a * a.a) * abxaa;
	float len2 = baxaaxabxaa.getSquaredLength();
	if(len2<EPSILON) return false;

	k = (((a_b*a.a)*(abxaa)) & baxaaxabxaa) / len2;

// nahodim t 

	v3 p = b.point(k);

	float abxaalen2 = abxaa.getSquaredLength();
	t = (((p-a.base)*a.a) & abxaa) / abxaalen2;

//  a.a*t = (p-a.base) + a.b*s

	s = (((p-a.base) + a.b*s)&a.a) / (a.a & a.a);

	return true;
}

// пересекает ли прямая (b.base + b.a*k) тругольник
// если да, то возвращается k
bool isCrossTrgLine(const Plane a, const Line b, float &k)
{
	float s,t;
	if(!isCross(a, b, s, t, k)) return false;
	return (s>=0)&&(t>=0)&&(s+t<=1);
}
