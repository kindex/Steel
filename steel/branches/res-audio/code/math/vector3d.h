/*id*********************************************************
    Unit: math/vector3d
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
        Steel Engine License
    Description:
		Class declaration for a 3d vector
************************************************************/

#ifndef __MATH_VECTOR3D_H
#define __MATH_VECTOR3D_H

#include "maths.h"

struct v3;

struct v3simple
{
	float x, y, z;

	float& operator[](int i)
	{
		return ((float*)(&x))[i];
	}
	const float& operator[](int i) const
	{
		return ((float*)(&x))[i];
	}
	operator v3() const;

	static inline v3simple get(float X, float Y, float Z) { v3simple temp; temp.x = X; temp.y = Y; temp.z = Z;  return temp; }
	static inline v3simple get(float *v) { v3simple temp; temp.x = v[0]; temp.y = v[1]; temp.z = v[2];  return temp; }

	//self-add etc
	inline void operator+=(const v3 & a);
	inline void operator-=(const v3 & a);
	inline void operator*=(const float a);
	inline void operator/=(const float a);
	inline void loadZero(void)	{	x = y = z = 0.0f;	}
	inline void loadOne(void)	{	x = y = z = 1.0f;	}

	//unary operators
	inline v3simple operator-(void) const {return get(-x, -y, -z);}
	inline v3simple operator+(void) const {return *this;}
};


struct v3
{
	//member variables
	float x, y, z;
	//constructors
	inline v3(void)	{} // не инициализирует ничего (для повышения производительности)
	inline v3(float X, float Y, float Z)	{ x = X; y = Y; z = Z; }

	inline void set(float X, float Y, float Z) {	x = X;	y = Y;	z = Z;	}
	inline void set(const v3 _v) {	x=_v.x;	y=_v.y; z=_v.z;	}
	
	inline void setX(float X) {x = X;}
	inline void setY(float Y) {y = Y;}
	inline void setZ(float Z) {z = Z;}

	static inline v3 get(float X, float Y, float Z) { v3 temp; temp.x = X; temp.y = Y; temp.z = Z;  return temp; }
	static inline v3 get(float *v) { v3 temp; temp.x = v[0]; temp.y = v[1]; temp.z = v[2];  return temp; }

	inline float getX() const {return x;}	//public accessor functions
	inline float getY() const {return y;}	//inline, const
	inline float getZ() const {return z;}

	// доступ к полям: 0 - x, 1 - y, 2 - z
	float& operator[](int i)
	{
		return ((float*)(&x))[i];
	}
	const float& operator[](int i) const
	{
		return ((float*)(&x))[i];
	}

	inline float* get3fv() const {return (float*)this;}

	inline void loadZero(void)	{	x=y=z=0.0f;	}
	inline void loadOne(void)	{	x=y=z=1.0f;	}
	
	//vector algebra
	inline v3 vectorProduct(const v3 & a) const
	{	v3 temp; temp.set(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x); return temp;	}

	inline v3 operator*(const v3 a) const	{	return vectorProduct(a);	}

	inline float dotProduct(const v3 & A) const	{	return x*A.x + y*A.y + z*A.z;	}
	inline float operator&(const v3 a) const		{	return dotProduct(a);	}

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
	inline v3 operator+(const v3 & a) const
	{	return get(x + a.x, y + a.y, z + a.z);	}
	
	inline v3 operator-(const v3 & a) const
	{	return get(x - a.x, y - a.y, z - a.z);	}

	inline v3 operator*(const float a) const
	{	return get(x*a, y*a, z*a);	}
	
	inline v3 operator/(const float a) const
	{	return (a==0.0f) ? get(0.0f, 0.0f, 0.0f) : get(x / a, y / a, z / a);	}

	//multiply by a float, eg 3*v
	inline friend v3 operator*(float scaleFactor, const v3 & a)	{	return a*scaleFactor;	}

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

	inline bool operator==(const v3 & a) const
	{
		return(fabs(x-a.x)<EPSILON && fabs(y-a.y)<EPSILON && fabs(z-a.z)<EPSILON);
	}

	inline bool operator!=(const v3 & a) const
	{
		return(fabs(x-a.x)>EPSILON || fabs(y-a.y)>EPSILON || fabs(z-a.z)>EPSILON);
	}

	//self-add etc
	inline void operator+=(const v3 & a)
	{	x+=a.x;	y+=a.y;	z+=a.z;	}

	inline void operator-=(const v3 & a)
	{	x-=a.x;	y-=a.y;	z-=a.z;	}

	inline void operator*=(const float a)
	{	x*=a;	y*=a;	z*=a;	}
	
	inline void operator/=(const float a)
	{	if(a==0.0f)
			return;
		else
		{	x/=a; y/=a; z/=a;	}
	}

	//unary operators
	inline v3 operator-(void) const {return get(-x, -y, -z);}
	inline v3 operator+(void) const {return *this;}

	//cast to pointer to a (float *) for glVertex3fv etc
	inline operator float* () const {return (float*) this;}
	inline operator const float* () const {return (const float*) this;}
};


//self-add etc
inline void v3simple::operator+=(const v3 & a)
{	x+=a.x;	y+=a.y;	z+=a.z;	}

inline void v3simple::operator-=(const v3 & a)
{	x-=a.x;	y-=a.y;	z-=a.z;	}

inline void v3simple::operator*=(const float a)
{	x*=a;	y*=a;	z*=a;	}

inline void v3simple::operator/=(const float a)
{	if(a==0.0f)
		return;
	else
	{	x/=a; y/=a; z/=a;	}
}


#endif	// __MATH_VECTOR3D_H
