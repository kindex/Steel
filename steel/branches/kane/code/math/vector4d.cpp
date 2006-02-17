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

#include "maths.h"
#include "vector4d.h"


void v4::RotateX(double angle)
{
	(*this)=GetRotatedX(angle);
}

v4 v4::GetRotatedX(double angle) const
{
	v3 v3d(x, y, z);

	v3d.RotateX(angle);

	return v4(v3d.x, v3d.y, v3d.z, w);
}

void v4::RotateY(double angle)
{
	(*this)=GetRotatedY(angle);
}

v4 v4::GetRotatedY(double angle) const
{
	v3 v3d(x, y, z);

	v3d.RotateY(angle);

	return v4(v3d.x, v3d.y, v3d.z, w);
}

void v4::RotateZ(double angle)
{
	(*this)=GetRotatedZ(angle);
}

v4 v4::GetRotatedZ(double angle) const
{
	v3 v3d(x, y, z);

	v3d.RotateZ(angle);

	return v4(v3d.x, v3d.y, v3d.z, w);
}

void v4::RotateAxis(double angle, const v3 & axis)
{
	(*this)=GetRotatedAxis(angle, axis);
}

v4 v4::GetRotatedAxis(double angle, const v3 & axis) const
{
	v3 v3d(x, y, z);

	v3d.RotateAxis(angle, axis);

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
