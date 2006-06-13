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
#ifndef __MATH__VECTOR2D_H
#define __MATH__VECTOR2D_H

#include "maths.h"
#include "../common/types.h"

class v2
{
public:
	//constructors
	v2(void)	:	x(0.0f), y(0.0f){}

	v2(float newX, float newY):	x(newX), y(newY)	{}

//	v2(const float * operand)		:	x(*operand), y((*operand)+1)	{}

	v2(const v2 & operand)	:	x(operand.x), y(operand.y)	{}

	~v2() {}	//empty

	void set(float newX, float newY){	x=newX;	y=newY;	}

	//Accessors kept for compatibility
	void setX(float newX) {x = newX;}
	void setY(float newY) {y = newY;}

	float getX() const {return x;}	//public accessor functions
	float getY() const {return y;}	//inline, const

	void loadZero(void) { x=0.0; y=0.0; }
	void loadOne(void);					//fill with (1, 1)

	void normalize(); // set length to 1
	v2 getNormalized() const;

	float getLength() const
	{	return (float)sqrt((x*x)+(y*y));	}

	float getSquaredLength() const
	{	return (x*x)+(y*y);	}

	//linear interpolate
	v2 lerp(const v2 & v2, float factor) const
	{	return (*this)*(1.0f-factor) + v2*factor;	}

//	v2 QuadraticInterpolate(const v2 & v2, const v2 & v3, float factor) const
//	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}

	//overloaded operators
	//binary operators
	v2 operator+(const v2 & operand) const 	{	return v2(x + operand.x, y + operand.y);	}
	v2 operator-(const v2 & operand) const 	{	return v2(x - operand.x, y - operand.y);	}
	v2 operator*(const float operand) const	{	return v2(x*operand, y*operand);	}

    float pseudoscalarProduct(v2 const &A) { return x*A.y-y*A.x; } // Psevdoskalarnoe proizvedenie vektorov
    float dotProduct(const v2 &A) { return x*A.x+y*A.y; } // skalarnoe proizvedenie vektorov

    friend bool operator==(const v2 &L, const v2 &R) { return (L.x==R.x) && (L.y==R.y); }

	v2 operator/(const float operand) const
	{	return (operand==0) ? v2(0.0f, 0.0f) : v2(x / operand, y / operand);	}

	//multiply by a float, eg 3*v
	friend v2 operator*(float scaleFactor, const v2 & operand) {return v2(operand.x*scaleFactor, operand.y*scaleFactor);}

//	bool operator==(const v2 & operand) const;
	bool operator!=(const v2 & operand) const	{	return !((*this)==operand);	}

	//self-add etc
	void operator+=(const v2 & operand)	{	x+=operand.x;	y+=operand.y;}
	void operator-=(const v2 & operand)	{	x-=operand.x;	y-=operand.y;}
	void operator*=(const float operand)     	{	x*=operand;	y*=operand;	}
	void operator/=(const float operand)
    {
        if(operand==0.0f)
			return;
		else
		{	x/=operand; y/=operand;	}
	}

	//unary operators
	v2 operator-(void) const {return v2(-x, -y);}
	v2 operator+(void) const {return *this;}

	//cast to pointer to a (float *) for glVertex3fv etc
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

    void rotate(float sinA, float cosA)
    {
        float xx = x * cosA - y * sinA;
        y = y * cosA + x * sinA;
        x = xx;
    }
    void rotate(float A)    { rotate(sin(A),cos(A));    }
    v2 rot90() { return v2(-y, x);}
    v2 rot_90() { return v2(y, -x);}

	//member variables
	float x, y;
};

#endif	//v2_H
