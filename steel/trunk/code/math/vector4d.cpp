//////////////////////////////////////////////////////////////////////////////////////////
//	v4.cpp
//	Function definitions for 4d vector class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	15th August 2002	-	prevent divide by zero in operator v3()
//	Modified:	8th November 2002	-	Changed Constructor layout
//									-	Some speed Improvements
//									-	Corrected Lerp
//////////////////////////////////////////////////////////////////////////////////////////	

#include "Maths.h"
#include "vector4d.h"


void v4::rotateX(double angle)
{
	(*this) = getRotatedX(angle);
}

v4 v4::getRotatedX(double angle) const
{
	v3 v3d(x, y, z);

	v3d.rotateX(angle);

	return v4(v3d.x, v3d.y, v3d.z, w);
}

void v4::rotateY(double angle)
{
	(*this) = getRotatedY(angle);
}

v4 v4::getRotatedY(double angle) const
{
	v3 v3d(x, y, z);

	v3d.rotateY(angle);

	return v4(v3d.x, v3d.y, v3d.z, w);
}

void v4::rotateZ(double angle)
{
	(*this) = getRotatedZ(angle);
}

v4 v4::getRotatedZ(double angle) const
{
	v3 v3d(x, y, z);

	v3d.rotateZ(angle);

	return v4(v3d.x, v3d.y, v3d.z, w);
}

void v4::rotateAxis(double angle, const v3 & axis)
{
	(*this) = getRotatedAxis(angle, axis);
}

v4 v4::getRotatedAxis(double angle, const v3 & axis) const
{
	v3 v3d(x, y, z);

	v3d.rotateAxis(angle, axis);

	return v4(v3d.x, v3d.y, v3d.z, w);
}


v4 operator*(float scaleFactor, const v4 & rhs)
{
	return rhs*scaleFactor;
}

bool v4::operator==(const v4 & rhs) const
{
	if(x==rhs.x && y==rhs.y && z==rhs.z && w==rhs.w)
		return true;

	return false;
}

v4::operator v3()
{
	if(w==0.0f || w==1.0f)
		return v3(x, y, z);
	else
		return v3(x/w, y/w, z/w);
}
