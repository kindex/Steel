/*id*********************************************************
    Unit: math/aabb
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	Licence:
        Только для Division
    Description:
		AABB (Axis-Aligned bounding box)
 ************************************************************/

#include "aabb.h"
#include "geometry.h"

void aabb::clear()
{ 
	min.x = +INF; 
	min.y = +INF; 
	min.z = +INF; 
	max.x = -INF; 
	max.y = -INF;
	max.z = -INF;
}

void aabb::add(v3 a)
{
	if(a.x>0) max.x += a.x; else min.x += a.x;
	if(a.y>0) max.y += a.y; else min.y += a.y;
	if(a.z>0) max.z += a.z; else min.z += a.z;
}

bool aabb::intersect(aabb const &second)
{
    return
	::intersect(min.x, max.x, second.min.x, second.max.x)
    &&
	::intersect(min.y, max.y, second.min.y, second.max.y)
	&&
	::intersect(min.z, max.z, second.min.z, second.max.z);
}

void aabb::mul(const matrix44 &matrix)
{
	std::vector<v3> v = getVertexes();
	
	clear();
	for(int i=0; i<8; i++)
		merge(matrix*v[i]);
}


void aabb::merge(const v3 point)
{
    if (min.x > point.x) min.x = point.x;
    if (min.y > point.y) min.y = point.y;
    if (min.z > point.z) min.z = point.z;

    if (max.x < point.x) max.x = point.x;
    if (max.y < point.y) max.y = point.y;
    if (max.z < point.z) max.z = point.z;
}

void aabb::merge(aabb const &second)
{
	if (min.x > second.min.x) min.x = second.min.x;
	if (min.y > second.min.y) min.y = second.min.y;
	if (min.z > second.min.z) min.z = second.min.z;

	if (max.x < second.max.x) max.x = second.max.x;
	if (max.y < second.max.y) max.y = second.max.y;
	if (max.z < second.max.z) max.z = second.max.z;
 }

std::vector<v3> aabb::getVertexes() const
{
	std::vector<v3> r;
	const v3 &a = min; 
	const v3 &b = max;
	
	r.push_back(v3(a.x, a.y, a.z));
	r.push_back(v3(a.x, a.y, b.z));
	r.push_back(v3(a.x, b.y, a.z));
	r.push_back(v3(a.x, b.y, b.z));
	r.push_back(v3(b.x, a.y, a.z));
	r.push_back(v3(b.x, a.y, b.z));
	r.push_back(v3(b.x, b.y, a.z));
	r.push_back(v3(b.x, b.y, b.z));

	return r;
}

void aabb::cross(const aabb3 b)
{
	if(b.min.x > min.x) min.x = b.min.x;
	if(b.min.y > min.y) min.y = b.min.y;
	if(b.min.z > min.z) min.z = b.min.z;

	if(b.max.x < max.x) max.x = b.max.x;
	if(b.max.y < max.y) max.y = b.max.y;
	if(b.max.z < max.z) max.z = b.max.z;
}
