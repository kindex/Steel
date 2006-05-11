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

struct v3
{
	//member variables
	float x, y, z;

	//constructors
	inline v3(void)	:	x(0.0f), y(0.0f), z(0.0f)	{}

	inline v3(float newX, float newY, float newZ)	:	x(newX), y(newY), z(newZ)	{}

	inline v3(const float * rhs)	:	x(*rhs), y(*(rhs+1)), z(*(rhs+2))	{}

	inline v3(const v3 & rhs)	:	x(rhs.x), y(rhs.y), z(rhs.z)	{}

	inline void set(float newX, float newY, float newZ) {	x=newX;	y=newY;	z=newZ;	}
	inline void set(const v3 _v) {	x=_v.x;	y=_v.y; z=_v.z;	}
	
	//Accessors kept for compatibility
	inline void setX(float newX) {x = newX;}
	inline void setY(float newY) {y = newY;}
	inline void setZ(float newZ) {z = newZ;}

	inline float getX() const {return x;}	//public accessor functions
	inline float getY() const {return y;}	//inline, const
	inline float getZ() const {return z;}

	inline void loadZero(void)	{	x=y=z=0.0f;	}
	inline void loadOne(void)	{	x=y=z=1.0f;	}
	
	//vector algebra
	inline v3 vectorProduct(const v3 & rhs) const
	{	return v3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);	}

	inline v3 operator*(const v3 rhs) const	{	return vectorProduct(rhs);	}

	inline float dotProduct(const v3 & A) const	{	return x*A.x + y*A.y + z*A.z;	}
	inline float operator&(const v3 rhs) const		{	return dotProduct(rhs);	}

	void normalize();
	v3 getNormalized() const;
	
	inline float getLength() const
	{	return (float)sqrt(((double)x*(double)x)+((double)y*(double)y)+((double)z*(double)z));	}
	inline double getLengthd() const
	{	return sqrt((double)x*(double)x + (double)y*(double)y + (double)z*(double)z);	}
	
	inline float getSquaredLength() const
	{	return (x*x)+(y*y)+(z*z);	}

	inline double getSquaredLengthd() const
	{	return ((double)x*(double)x)+((double)y*(double)y)+((double)z*(double)z);	}

	//rotations
	void rotateX(double angle);
	v3 getRotatedX(double angle) const;
	void rotateY(double angle);
	v3 getRotatedY(double angle) const;
	void rotateZ(double angle);
	v3 getRotatedZ(double angle) const;
	void rotateAxis(double angle, const v3 & axis);
	v3 getRotatedAxis(double angle, const v3 & axis) const;

	//pack to [0,1] for color
	void packTo01();
	v3 getPackedTo01() const;

	//linear interpolate
	v3 lerp(const v3 & v2, float factor) const
	{	return (*this)*(1.0f-factor) + v2*factor;	}

	v3 quadraticInterpolate(const v3 & v2, const v3 & v3, float factor) const
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}


	//overloaded operators
	//binary operators
	inline v3 operator+(const v3 & rhs) const
	{	return v3(x + rhs.x, y + rhs.y, z + rhs.z);	}
	
	inline v3 operator-(const v3 & rhs) const
	{	return v3(x - rhs.x, y - rhs.y, z - rhs.z);	}

	inline v3 operator*(const float rhs) const
	{	return v3(x*rhs, y*rhs, z*rhs);	}
	
	inline v3 operator/(const float rhs) const
	{	return (rhs==0.0f) ? v3(0.0f, 0.0f, 0.0f) : v3(x / rhs, y / rhs, z / rhs);	}

	//multiply by a float, eg 3*v
	inline friend v3 operator*(float scaleFactor, const v3 & rhs)	{	return rhs*scaleFactor;	}


	//Add, subtract etc, saving the construction of a temporary
	inline void add(const v3 & v2, v3 & result)
	{
		result.x=x+v2.x;
		result.y=y+v2.y;
		result.z=z+v2.z;
	}

	inline void subtract(const v3 & v2, v3 & result)
	{
		result.x=x-v2.x;
		result.y=y-v2.y;
		result.z=z-v2.z;
	}

	inline bool operator==(const v3 & rhs) const
	{
		return(fabs(x-rhs.x)<EPSILON && fabs(y-rhs.y)<EPSILON && fabs(z-rhs.z)<EPSILON);
	}

	inline bool operator!=(const v3 & rhs) const
	{
		return(fabs(x-rhs.x)>EPSILON || fabs(y-rhs.y)>EPSILON || fabs(z-rhs.z)>EPSILON);
	}

	//self-add etc
	inline void operator+=(const v3 & rhs)
	{	x+=rhs.x;	y+=rhs.y;	z+=rhs.z;	}

	inline void operator-=(const v3 & rhs)
	{	x-=rhs.x;	y-=rhs.y;	z-=rhs.z;	}

	inline void operator*=(const float rhs)
	{	x*=rhs;	y*=rhs;	z*=rhs;	}
	
	inline void operator/=(const float rhs)
	{	if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs; z/=rhs;	}
	}


	//unary operators
	inline v3 operator-(void) const {return v3(-x, -y, -z);}
	inline v3 operator+(void) const {return *this;}

	//cast to pointer to a (float *) for glVertex3fv etc
	inline operator float* () const {return (float*) this;}
	inline operator const float* () const {return (const float*) this;}

};

#endif	//v3_H
