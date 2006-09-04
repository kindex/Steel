/*id*********************************************************
	File: math/vector2d.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Class declaration for a 2d vector
************************************************************/

#include "maths.h"
#include "vector2d.h"

void v2::normalize()
{
	float length;
	length = getLength();

	if(length == 0)			//return if length is 1 or 0
		return;

	x /= length;
	y /= length;
}

v2 v2::getNormalized() const
{
	v2 result(*this);

	result.normalize();

	return result;
}

/*v2 operator*(float scaleFactor, const v2 & operand)
{
	return operand*scaleFactor;
}*/

