//////////////////////////////////////////////////////////////////////////////////////////
//	v3.cpp
//	Function definitions for 3d vector class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	8th November 2002	-	Changed Constructor layout
//									-	Some speed Improvements
//									-	Corrected Lerp
//				17th December 2002	-	Converted from radians to degrees
//////////////////////////////////////////////////////////////////////////////////////////	

#include "Maths.h"
#include "vector3d.h"

void v3::Normalize()
{
	float length=GetLength();

	if(length==1 || length==0)			//return if length is 1 or 0
		return;

	float scalefactor = 1.0f/length;
	x *= scalefactor;
	y *= scalefactor;
	z *= scalefactor;
}

v3 v3::GetNormalized() const
{
	v3 result(*this);

	result.Normalize();

	return result;
}

v3 v3::GetRotatedX(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);

	return v3(	x,
						y*cosAngle - z*sinAngle,
						y*sinAngle + z*cosAngle);
}

void v3::RotateX(double angle)
{
	(*this)=GetRotatedX(angle);
}

v3 v3::GetRotatedY(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);

	return v3(	x*cosAngle + z*sinAngle,
						y,
						-x*sinAngle + z*cosAngle);
}

void v3::RotateY(double angle)
{
	(*this)=GetRotatedY(angle);
}

v3 v3::GetRotatedZ(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);
	
	return v3(x*cosAngle - y*sinAngle,
					x*sinAngle + y*cosAngle,
					z);
}

void v3::RotateZ(double angle)
{
	(*this)=GetRotatedZ(angle);
}

v3 v3::GetRotatedAxis(double angle, const v3 & axis) const
{
	if(angle==0.0)
		return (*this);

	v3 u=axis.GetNormalized();

	v3 rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);
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

	return v3(	this->dotProduct(rotMatrixRow0),
						this->dotProduct(rotMatrixRow1),
						this->dotProduct(rotMatrixRow2));
}

void v3::RotateAxis(double angle, const v3 & axis)
{
	(*this)=GetRotatedAxis(angle, axis);
}


void v3::PackTo01()
{
	(*this)=GetPackedTo01();	
}

v3 v3::GetPackedTo01() const
{
	v3 temp(*this);

	temp.Normalize();

	temp=temp*0.5f+v3(0.5f, 0.5f, 0.5f);
	
	return temp;
}

v3 operator*(float scaleFactor, const v3 & rhs)
{
	return rhs*scaleFactor;
}

bool v3::operator==(const v3 & rhs) const
{
	if(x==rhs.x && y==rhs.y && z==rhs.z)
		return true;

	return false;
}

