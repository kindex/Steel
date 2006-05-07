#ifndef __MATH_AABB_H
#define __MATH_AABB_H

// AABB (Axis-Aligned bounding box)

#include "vector3d.h"
#include "matrix4x4.h"

#include <vector>

struct aabb3 // AABB 3D
{
	v3 min, max;

	aabb3() { clear(); }
	void clear();
	std::vector<v3> getVertexes();

    void merge(const v3 point);
    void merge(const aabb3 &second);
	bool intersect(aabb3 const &second);
	void mul(const matrix44 &matrix);
	void add(v3 direction);
};

typedef aabb3 aabb;

#endif
