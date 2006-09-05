/*id*********************************************************
	File: math/aabb.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		AABB (Axis-Aligned bounding box)
************************************************************/

#ifndef __MATH_AABB_H
#define __MATH_AABB_H

#include "vector3d.h"
#include "matrix34.h"

#include "../common/steel_vector.h"

struct aabb3 // AABB 3D
{
	v3 min, max;

	aabb3(void) { clear(); }
	aabb3(const v3 _min, const v3 _max): min(_min), max(_max) {}
	aabb3(const v3 point): min(point), max(point) {}
	bool empty(void) const { return min.x>max.x + EPSILON || min.y>max.y + EPSILON || min.z>max.z + EPSILON;  }
	void clear(void);
	void getVertexes(steel::svector<v3> &dest) const;

    void merge(const v3 point);
    void merge(const aabb3 &second);
	bool intersect(aabb3 const &second);
	void mul(const matrix34 &matrix);
	void add(v3 direction);
	void cross(const aabb3 second);
};

typedef aabb3 aabb;

#endif
