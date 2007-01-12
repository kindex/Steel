/*id*********************************************************
	File: math/aabb.h
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

#ifndef __MATH_AABB_H
#define __MATH_AABB_H

#include "vector3d.h"
#include "matrix34.h"

#include "../common/types.h"
#undef min
#undef max

#undef min
#undef max

struct AABB3 // AABB 3D
{
	v3 min, max;

	AABB3() { clear(); }
	AABB3(const v3 _min, const v3 _max): min(_min), max(_max) {}
	AABB3(const v3 point): min(point), max(point) {}
	bool empty() const { return min.x>max.x + EPSILON || min.y>max.y + EPSILON || min.z>max.z + EPSILON;  }
	void clear();
	void getVertexes(v3Vector &dest) const;

    void merge(const v3 point);
    void merge(const AABB3 &second);
	bool intersect(AABB3 const &second);
	void mul(const matrix34 &matrix);
	void add(v3 direction);
	void cross(const AABB3 second);
	bool isCrossingSphere(const v3 center, const float radiuss);
};

typedef AABB3 AABB;

#endif
