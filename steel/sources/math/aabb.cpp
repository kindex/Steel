/*id*********************************************************
	File: math/aabb.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		AABB (Axis-Aligned bounding box)
************************************************************/

#include "aabb.h"


bool intersect(float amin, float amax, float bmin, float bmax)
{
	if(bmin > amin) amin = bmin;
	if(bmax < amax) amax = bmax;

    return
		amin  < amax+ EPSILON;
}

void AABB3::clear()
{ 
	min.x = +INF; 
	min.y = +INF; 
	min.z = +INF; 
	max.x = -INF; 
	max.y = -INF;
	max.z = -INF;
}

void AABB3::add(v3 a)
{
	if(a.x>0) max.x += a.x; else min.x += a.x;
	if(a.y>0) max.y += a.y; else min.y += a.y;
	if(a.z>0) max.z += a.z; else min.z += a.z;
}

bool AABB3::intersect(AABB3 const &second)
{
    return
	::intersect(min.x, max.x, second.min.x, second.max.x)
    &&
	::intersect(min.y, max.y, second.min.y, second.max.y)
	&&
	::intersect(min.z, max.z, second.min.z, second.max.z);
}

void AABB3::mul(const matrix34 &matrix)
{
	pvector<v3> v;
	getVertexes(v);
	
	clear();
	for(int i=0; i<8; i++)
	{
		merge(matrix*v[i]);
	}
}


void AABB3::merge(const v3 point)
{
    if (min.x > point.x) min.x = point.x;
    if (min.y > point.y) min.y = point.y;
    if (min.z > point.z) min.z = point.z;

    if (max.x < point.x) max.x = point.x;
    if (max.y < point.y) max.y = point.y;
    if (max.z < point.z) max.z = point.z;
}

void AABB3::merge(AABB const &second)
{
	if (min.x > second.min.x) min.x = second.min.x;
	if (min.y > second.min.y) min.y = second.min.y;
	if (min.z > second.min.z) min.z = second.min.z;

	if (max.x < second.max.x) max.x = second.max.x;
	if (max.y < second.max.y) max.y = second.max.y;
	if (max.z < second.max.z) max.z = second.max.z;
 }

void AABB3::getVertexes(OUT pvector<v3>& dest) const
{
	dest.resize(8);
	const v3 &a = min; 
	const v3 &b = max;
	
	dest[0].set(a.x, a.y, a.z);
	dest[1].set(a.x, a.y, b.z);
	dest[2].set(a.x, b.y, a.z);
	dest[3].set(a.x, b.y, b.z);
	dest[4].set(b.x, a.y, a.z);
	dest[5].set(b.x, a.y, b.z);
	dest[6].set(b.x, b.y, a.z);
	dest[7].set(b.x, b.y, b.z);
}

void AABB3::cross(const AABB3 b)
{
	if(b.min.x > min.x) min.x = b.min.x;
	if(b.min.y > min.y) min.y = b.min.y;
	if(b.min.z > min.z) min.z = b.min.z;

	if(b.max.x < max.x) max.x = b.max.x;
	if(b.max.y < max.y) max.y = b.max.y;
	if(b.max.z < max.z) max.z = b.max.z;
}

// false when not crossing 
// true crossing or not crossing
bool AABB3::isCrossingSphere(const v3 center, const float radiuss)
{
	AABB3 aabb(center - v3(radiuss, radiuss, radiuss), center + v3(radiuss, radiuss, radiuss));
	return this->intersect(aabb);
}
