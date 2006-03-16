//////////////////////////////////////////////////////////////////////////////////////////
//	v3.h
//	Class declaration for a 3d vector
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	8th November 2002	-	Changed Constructor layout
//									-	Some speed Improvements
//									-	Corrected Lerp
//				7th January 2003	-	Added QuadraticInterpolate
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef v3_H
#define v3_H

#include "maths.h"

class v3
{
public:
	//constructors
	v3(void)	:	x(0.0f), y(0.0f), z(0.0f)
	{}

	v3(float newX, float newY, float newZ)	:	x(newX), y(newY), z(newZ)
	{}

	v3(const float * rhs)	:	x(*rhs), y(*(rhs+1)), z(*(rhs+2))
	{}

	v3(const v3 & rhs)	:	x(rhs.x), y(rhs.y), z(rhs.z)
	{}

	~v3() {}	//empty

	void Set(float newX, float newY, float newZ)
	{	x=newX;	y=newY;	z=newZ;	}
	
	//Accessors kept for compatibility
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}
	void SetZ(float newZ) {z = newZ;}

	float GetX() const {return x;}	//public accessor functions
	float GetY() const {return y;}	//inline, const
	float GetZ() const {return z;}

	void LoadZero(void)
	{	x=y=z=0.0f;	}
	void LoadOne(void)
	{	x=y=z=1.0f;	}
	
	//vector algebra
	v3 vectorProduct(const v3 & rhs) const
	{	return v3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);	}

	float dotProduct(const v3 & A) const
	{	return x*A.x + y*A.y + z*A.z;	}

	float operator*(const v3 & A) const {return dotProduct(A);}

	void Normalize();
	v3 GetNormalized() const;
	
	float GetLength() const
	{	return (float)sqrt((x*x)+(y*y)+(z*z));	}
	
	float GetSquaredLength() const
	{	return (x*x)+(y*y)+(z*z);	}

	//rotations
	void RotateX(double angle);
	v3 GetRotatedX(double angle) const;
	void RotateY(double angle);
	v3 GetRotatedY(double angle) const;
	void RotateZ(double angle);
	v3 GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const v3 & axis);
	v3 GetRotatedAxis(double angle, const v3 & axis) const;

	//pack to [0,1] for color
	void PackTo01();
	v3 GetPackedTo01() const;

	//linear interpolate
	v3 lerp(const v3 & v2, float factor) const
	{	return (*this)*(1.0f-factor) + v2*factor;	}

	v3 QuadraticInterpolate(const v3 & v2, const v3 & v3, float factor) const
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}


	//overloaded operators
	//binary operators
	v3 operator+(const v3 & rhs) const
	{	return v3(x + rhs.x, y + rhs.y, z + rhs.z);	}
	
	v3 operator-(const v3 & rhs) const
	{	return v3(x - rhs.x, y - rhs.y, z - rhs.z);	}

	v3 operator*(const float rhs) const
	{	return v3(x*rhs, y*rhs, z*rhs);	}
	
	v3 operator/(const float rhs) const
	{	return (rhs==0.0f) ? v3(0.0f, 0.0f, 0.0f) : v3(x / rhs, y / rhs, z / rhs);	}

	//multiply by a float, eg 3*v
	friend v3 operator*(float scaleFactor, const v3 & rhs);

	//Add, subtract etc, saving the construction of a temporary
	void Add(const v3 & v2, v3 & result)
	{
		result.x=x+v2.x;
		result.y=y+v2.y;
		result.z=z+v2.z;
	}

	void Subtract(const v3 & v2, v3 & result)
	{
		result.x=x-v2.x;
		result.y=y-v2.y;
		result.z=z-v2.z;
	}

	bool operator==(const v3 & rhs) const;
	bool operator!=(const v3 & rhs) const
	{	return !((*this)==rhs);	}

	//self-add etc
	void operator+=(const v3 & rhs)
	{	x+=rhs.x;	y+=rhs.y;	z+=rhs.z;	}

	void operator-=(const v3 & rhs)
	{	x-=rhs.x;	y-=rhs.y;	z-=rhs.z;	}

	void operator*=(const float rhs)
	{	x*=rhs;	y*=rhs;	z*=rhs;	}
	
	void operator/=(const float rhs)
	{	if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs; z/=rhs;	}
	}


	//unary operators
	v3 operator-(void) const {return v3(-x, -y, -z);}
	v3 operator+(void) const {return *this;}

	//cast to pointer to a (float *) for glVertex3fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	//member variables
	float x;
	float y;
	float z;
};

#endif	//v3_H