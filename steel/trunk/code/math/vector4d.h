//////////////////////////////////////////////////////////////////////////////////////////
//	v4.h
//	Class declaration for a 4d vector
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//	Modified:	8th November 2002	-	Changed Constructor layout
//									-	Some speed Improvements
//									-	Corrected Lerp
//				7th January 2003	-	Added QuadraticInterpolate
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef v4_H
#define v4_H

#include "vector3d.h"

class v4
{
public:
	//constructors
	v4(void)	:	x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}

	v4(float newX, float newY, float newZ, float newW)	
		:	x(newX), y(newY), z(newZ), w(newW)
	{}

	v4(const float * rhs)	:	x(*rhs), y(*(rhs+1)), z(*(rhs+2)), w(*(rhs+3))
	{}

	v4(const v4 & rhs):	x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
	{}

	//convert v3d to v4d
	v4(const v3 & rhs):	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
	{}

	~v4() {}	//empty

	void set(float newX, float newY, float newZ, float newW)
	{	x=newX;	y=newY;	z=newZ; w=newW;	}
	
	//accessors kept for compatability
	void setX(float newX) {x = newX;}
	void setY(float newY) {y = newY;}
	void setZ(float newZ) {z = newZ;}
	void setW(float newW) {w = newW;}
	
	float getX() const {return x;}	//public accessor functions
	float getY() const {return y;}	//inline, const
	float getZ() const {return z;}
	float getW() const {return w;}

	void loadZero(void)
	{	x=0.0f; y=0.0f; z=0.0f; w=0.0f;	}

	void loadOne(void)
	{	x=1.0f; y=1.0f; z=1.0f; w=1.0f;	}

	//vector algebra
	float dotProduct(const v4 & rhs)
	{	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;	}

	//rotations
	void rotateX(double angle);
	v4 getRotatedX(double angle) const;
	void rotateY(double angle);
	v4 getRotatedY(double angle) const;
	void rotateZ(double angle);
	v4 getRotatedZ(double angle) const;
	void rotateAxis(double angle, const v3 & axis);
	v4 getRotatedAxis(double angle, const v3 & axis) const;
	
	v4 lerp(const v4 & v2, float factor) const
	{	return (*this)*(1.0f-factor)+v2*factor;	}

	v4 quadraticInterpolate(const v4 & v2, const v4 & v3, float factor) const
	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}

	//binary operators
	v4 operator+(const v4 & rhs) const
	{	return v4(x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w);	}

	v4 operator-(const v4 & rhs) const
	{	return v4(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w);	}

	v4 operator*(const float rhs) const
	{	return v4(x*rhs, y*rhs, z*rhs, w*rhs);	}

	v4 operator/(const float rhs) const
	{	return rhs==0.0f	?	v4(0.0f, 0.0f, 0.0f, 0.0f)
							:	v4(x/rhs, y/rhs, z/rhs, w/rhs);	}

	//multiply by a float, eg 3*v
	friend v4 operator*(float scaleFactor, const v4 & rhs);

	bool operator==(const v4 & rhs) const;
	bool operator!=(const v4 & rhs) const
	{	return !((*this)==rhs);	}

	//self-add etc
	void operator+=(const v4 & rhs)
	{	x+=rhs.x; y+=rhs.y; z+=rhs.z; w+=rhs.w;	}

	void operator-=(const v4 & rhs)
	{	x-=rhs.x; y-=rhs.y; z-=rhs.z; w-=rhs.w;	}

	void operator*=(const float rhs)
	{	x*=rhs; y*=rhs; z*=rhs; w*=rhs;	}

	void operator/=(const float rhs)
	{	if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs; z/=rhs; w/=rhs;	}
	}

	//unary operators
	v4 operator-(void) const {return v4(-x, -y, -z, -w);}
	v4 operator+(void) const {return (*this);}

	//cast to pointer to float for glVertex4fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	operator v3();							//convert v4d to v3d

	//member variables
	float x;
	float y;
	float z;
	float w;
};

#endif	//v3_H
