#include "plane.h"

/*
Находится ли точка между лучами (base + a*t) и (base + b*t)
*/
bool isBetween(v3 point, const v3 base, const v3 a, const v3 b)
{
	point -= base;
	v3 ab = a*b;
	float k1 =(ab)&(a*point);
	float k2 =(ab)&(point*b);
	return k1>-EPSILON && k2>-EPSILON;
}

bool isCross(const Plane a, const Line b, float &k)
{
// a.base + a.a*s + a.b*t = b.base + b.a * k

// nahodim k

	v3 a_b = (a.base-b.base);
	v3 abxaa = a.b*a.a; 
	v3 baxaaxabxaa =  (b.a * a.a) * abxaa;
	double len2 = baxaaxabxaa.getSquaredLengthd();
	if(len2<EPSILON2) return false;

	k = (((a_b*a.a)*(abxaa)) & baxaaxabxaa) / len2;

	return true;
}

// пересекает ли прямая (b.base + b.a*k) тругольник
// если да, то возвращается k
bool isCrossTrgLine(const Plane a, const Line b, float &k)
{
	if(!isCross(a, b, k)) return false;

	v3 p = b.point(k);

	return a.isInTrg(p);
}


bool Plane::isInTrg(v3 p) const
{
	return isBetween(p, base, a, b) && isBetween(p, base + a, b - a, -a);
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
	
	if(!isCross(p, b, k)) return false;
	if(k<-EPSILON || k>1+ EPSILON) return false;

	v3 point = b.point(k);

	return  isBetween(point, a.base, direction, a.a) && 
			isBetween(point, a.base + a.a + direction, -a.a, -direction);
}
