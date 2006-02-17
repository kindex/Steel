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

#include "maths.h"
#include "vector2d.h"

void v2::Normalize()
{
	coord length;
	coord scalefactor;
	length=GetLength();

	if(length==1 || length==0)			//return if length is 1 or 0
		return;

	scalefactor = 1.0f/length;
	x *= scalefactor;
	y *= scalefactor;
}

v2 v2::GetNormalized() const
{
	v2 result(*this);

	result.Normalize();

	return result;
}

/*v2 operator*(float scaleFactor, const v2 & rhs)
{
	return rhs*scaleFactor;
}*/
