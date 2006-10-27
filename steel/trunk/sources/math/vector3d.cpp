/*id*********************************************************
	File: math/vector3d.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Class declaration for a 3d vector
************************************************************/

#include "maths.h"
#include "vector3d.h"

v3simple::operator v3() const
{ 
	return v3(x, y, z);
}

void v3::normalize()
{
	float length=getLength();

	if(length==1 || length==0)			//return if length is 1 or 0
		return;

	float scalefactor = 1.0f/length;
	x *= scalefactor;
	y *= scalefactor;
	z *= scalefactor;
}

v3 v3::getNormalized() const
{
	v3 result(*this);

	result.normalize();

	return result;
}

v3 v3::getRotatedX(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(angle);
	float cosAngle=(float)cos(angle);

	return get(	x,
						y*cosAngle - z*sinAngle,
						y*sinAngle + z*cosAngle);
}

void v3::rotateX(double angle)
{
	(*this)=getRotatedX(angle);
}

v3 v3::getRotatedY(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(angle);
	float cosAngle=(float)cos(angle);

	return get(	x*cosAngle + z*sinAngle,
						y,
						-x*sinAngle + z*cosAngle);
}

void v3::rotateY(double angle)
{
	(*this) = getRotatedY(angle);
}

v3 v3::getRotatedZ(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(angle);
	float cosAngle=(float)cos(angle);
	
	return get(x*cosAngle - y*sinAngle,
					x*sinAngle + y*cosAngle,
					z);
}

void v3::rotateZ(double angle)
{
	(*this) = getRotatedZ(angle);
}

v3 v3::getRotatedAxis(double angle, const v3 & axis) const
{
	if(angle == 0.0)
		return (*this);

	v3 u=axis.getNormalized();

	v3 rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;

	float sinAngle=(float)sin(angle);
	float cosAngle=(float)cos(angle);
	float oneMinusCosAngle=1.0f-cosAngle;

	rotMatrixRow0.x=(u.x)*(u.x) + cosAngle*(1-(u.x)*(u.x));
	rotMatrixRow0.y=(u.x)*(u.y)*(oneMinusCosAngle) - sinAngle*u.z;
	rotMatrixRow0.z=(u.x)*(u.z)*(oneMinusCosAngle) + sinAngle*u.y;

	rotMatrixRow1.x=(u.x)*(u.y)*(oneMinusCosAngle) + sinAngle*u.z;
	rotMatrixRow1.y=(u.y)*(u.y) + cosAngle*(1-(u.y)*(u.y));
	rotMatrixRow1.z=(u.y)*(u.z)*(oneMinusCosAngle) - sinAngle*u.x;
	
	rotMatrixRow2.x=(u.x)*(u.z)*(oneMinusCosAngle) - sinAngle*u.y;
	rotMatrixRow2.y=(u.y)*(u.z)*(oneMinusCosAngle) + sinAngle*u.x;
	rotMatrixRow2.z=(u.z)*(u.z) + cosAngle*(1-(u.z)*(u.z));

	return get(	this->dotProduct(rotMatrixRow0),
						this->dotProduct(rotMatrixRow1),
						this->dotProduct(rotMatrixRow2));
}

void v3::rotateAxis(double angle, const v3 & axis)
{
	(*this) = getRotatedAxis(angle, axis);
}


void v3::packTo01()
{
	(*this)=getPackedTo01();	
}

v3 v3::getPackedTo01() const
{
	v3 temp(*this);

	temp.normalize();

	temp = temp*0.5f + get(0.5f, 0.5f, 0.5f);
	
	return temp;
}

