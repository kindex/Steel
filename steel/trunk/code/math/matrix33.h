/*id*********************************************************
    Unit: math/matrix3x3
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Description:
		Class declaration for a 3x4 matrix
		3 Rows x 3 Columns
		| 0 1 2 |
		| 3 4 5 |
		| 6 7 8 |
************************************************************/

#ifndef __MATRIX33_H
#define __MATRIX33_H

#include "vector3d.h"

struct matrix33
{
	//member variables
	union
	{
		float a[9]; // 1D array
		float m[3][3]; // 2D array - matrix 4 rows x 3 cols
//		v3simple v[3];
	} data;

//	inline matrix33(void) {}
//	matrix33(const float _00,	const float _01,	const float _02,	const float _10,	const float _11,	const float _12,	const float _20,	const float _21,	const float _22);

	void loadIdentity(void);
	void loadZero(void);

	void operator+=(const matrix33 operand);
	inline matrix33 operator+(const matrix33 operand) const	{ matrix33 m = *this; m += operand; return m; }

	void operator-=(const matrix33 operand);
	inline matrix33 operator-(const matrix33 operand) const	{ matrix33 m = *this; m -= operand; return m; }

	void operator*=(const float operand);
	void operator*=(const matrix33 operand);
	friend void operator*=(v3 &operand, const matrix33 operand2);
	friend v3 operator*(const v3 operand, const matrix33 operand2) 
	{
		v3 temp = operand; temp *= operand2; return temp;
	}

	inline matrix33 operator*(const float operand) const	{ matrix33 m = *this; m *= operand; return m; }
	inline matrix33 operator*(const matrix33 operand) const	{ matrix33 m = *this; m *= operand; return m; }
	v3 operator*(const v3 operand) const;

	void operator/=(const float operand);
	inline matrix33 operator/(const float operand) const	{ matrix33 m = *this; m /= operand; return m; }


	// in radians
	inline void setRotationX(const float angle) { setRotationX(sin(angle), cos(angle)); }
	void setRotationX(const float sin, const float cos);
	inline void setRotationY(const float angle) { setRotationY(sin(angle), cos(angle)); }
	void setRotationY(const float sin, const float cos);
	inline void setRotationZ(const float angle) { setRotationZ(sin(angle), cos(angle)); }
	void setRotationZ(const float sin, const float cos);

	inline void setRotationAxis(const float angle, const v3 axis)	{		setRotationAxis(sin(angle), sin(angle), axis);	}
	void setRotationAxis(const float sinAngle, const float cosAngle, const v3 axis);

	void setScale(const v3 scale);
	matrix33 getInverse(void) const;
};

#endif
