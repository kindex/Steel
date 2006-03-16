/****************************************************************
  TheGame 2D geometry unit (Steel engine)
  (C) KindeX (Andrey Ivanov), kindex@inbox.lv, http://kindex.lv
  Division, 2004-2005.
******************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////
//	based on v2.h
//	Class declaration for a 2d vector
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	6th November 2002
//	Modified:	7th January 2003	-	Added QuadraticInterpolate
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MATH_V2_H
#define __MATH_V2_H

#include "maths.h"

class v2
{
public:
	//constructors
	v2(void)	:	x(0.0f), y(0.0f){}

	v2(coord newX, coord newY):	x(newX), y(newY)	{}

//	v2(const coord * rhs)		:	x(*rhs), y((*rhs)+1)	{}

	v2(const v2 & rhs)	:	x(rhs.x), y(rhs.y)	{}

	~v2() {}	//empty

	void Set(coord newX, coord newY){	x=newX;	y=newY;	}

	//Accessors kept for compatibility
	void SetX(coord newX) {x = newX;}
	void SetY(coord newY) {y = newY;}

	coord GetX() const {return x;}	//public accessor functions
	coord GetY() const {return y;}	//inline, const

	void LoadZero(void) { x=0.0; y=0.0; }
	void LoadOne(void);					//fill with (1, 1)

	void Normalize(); // set length to 1
	v2 GetNormalized() const;

	coord GetLength() const
	{	return (coord)sqrt((x*x)+(y*y));	}

	coord GetSquaredLength() const
	{	return (x*x)+(y*y);	}

	//linear interpolate
	v2 lerp(const v2 & v2, coord factor) const
	{	return (*this)*(1.0f-factor) + v2*factor;	}

//	v2 QuadraticInterpolate(const v2 & v2, const v2 & v3, coord factor) const
//	{	return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;}

	//overloaded operators
	//binary operators
	v2 operator+(const v2 & rhs) const 	{	return v2(x + rhs.x, y + rhs.y);	}
	v2 operator-(const v2 & rhs) const 	{	return v2(x - rhs.x, y - rhs.y);	}
	v2 operator*(const coord rhs) const	{	return v2(x*rhs, y*rhs);	}

    coord pseudoscalarProduct(v2 const &A) { return x*A.y-y*A.x; } // Psevdoskalarnoe proizvedenie vektorov
    coord dotProduct(const v2 &A) { return x*A.x+y*A.y; } // skalarnoe proizvedenie vektorov

    friend bool operator==(const v2 &L, const v2 &R) { return (L.x==R.x) && (L.y==R.y); }

	v2 operator/(const coord rhs) const
	{	return (rhs==0) ? v2(0.0f, 0.0f) : v2(x / rhs, y / rhs);	}

	//multiply by a coord, eg 3*v
	friend v2 operator*(coord scaleFactor, const v2 & rhs) {return v2(rhs.x*scaleFactor, rhs.y*scaleFactor);}

//	bool operator==(const v2 & rhs) const;
	bool operator!=(const v2 & rhs) const	{	return !((*this)==rhs);	}

	//self-add etc
	void operator+=(const v2 & rhs)	{	x+=rhs.x;	y+=rhs.y;}
	void operator-=(const v2 & rhs)	{	x-=rhs.x;	y-=rhs.y;}
	void operator*=(const coord rhs)     	{	x*=rhs;	y*=rhs;	}
	void operator/=(const coord rhs)
    {
        if(rhs==0.0f)
			return;
		else
		{	x/=rhs; y/=rhs;	}
	}

	//unary operators
	v2 operator-(void) const {return v2(-x, -y);}
	v2 operator+(void) const {return *this;}

	//cast to pointer to a (coord *) for glVertex3fv etc
	operator coord* () const {return (coord*) this;}
	operator const coord* () const {return (const coord*) this;}

    void rotate(coord sinA, coord cosA)
    {
        coord xx = x * cosA - y * sinA;
        y = y * cosA + x * sinA;
        x = xx;
    }
    void rotate(coord A)    { rotate(sin(A),cos(A));    }
    v2 rot90() { return v2(-y, x);}
    v2 rot_90() { return v2(y, -x);}

	//member variables
	coord x, y;
};

#endif	//v2_H