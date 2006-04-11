/****************************************************************
  TheGame 2D geometry unit (Steel engine)
  (C) KindeX (Andrey Ivanov), kindex@inbox.lv, http://kindex.lv
  Division, 2004-2005.
******************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////
//	based on v2.cpp
//	Function definitions for 2d vector class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	8th November 2002
//////////////////////////////////////////////////////////////////////////////////////////

#include "Maths.h"
#include "vector2d.h"

void v2::normalize()
{
	coord length;
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

/*v2 operator*(float scaleFactor, const v2 & rhs)
{
	return rhs*scaleFactor;
}*/

