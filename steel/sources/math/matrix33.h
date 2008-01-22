/*id*********************************************************
	File: math/matrix33.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Class declaration for a 3x3 matrix
		3 Rows x 3 Columns
		| 0 1 2 |
		| 3 4 5 |
		| 6 7 8 |
************************************************************/

#ifndef __MATRIX33_H
#define __MATRIX33_H

#include "../steel.h"
#include "vector3d.h"

struct matrix33
{
	typedef float m33[3][3];
	//member variables
	union
	{
		float a[9]; // 1D array
		m33 m; // 2D array - matrix 3 rows x 3 cols, first index - row number, second - column number
//		v3simple v[3];
	} data;

//	inline matrix33() {}
//	matrix33(const float _00,	const float _01,	const float _02,	const float _10,	const float _11,	const float _12,	const float _20,	const float _21,	const float _22);

	void loadIdentity();
	void loadZero();

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


	// делают матрицу матрицей поворота вдоль указанной оси на указанный угол в радианах
	inline void setRotationX(const float angle) { setRotationX(sin(angle), cos(angle)); }
	void setRotationX(const float sin, const float cos);
	inline void setRotationY(const float angle) { setRotationY(sin(angle), cos(angle)); }
	void setRotationY(const float sin, const float cos);
	inline void setRotationZ(const float angle) { setRotationZ(sin(angle), cos(angle)); }
	void setRotationZ(const float sin, const float cos);

	inline void setRotationAxis(const float angle, const v3 axis)	{		setRotationAxis(sin(angle), sin(angle), axis);	}
	void setRotationAxis(const float sinAngle, const float cosAngle, const v3 axis);

	// setScale – делает матрицу масштабирования
	void setScale(const v3 scale);
	v3 getScale() const;

	// возвращает обратнрую матрицу
	matrix33 getInverse() const
	{
		matrix33 temp;
		getInverse(temp);
		return temp;
	}
	void getInverse(matrix33 &operand) const;
};

v3 operator*(const v3& operand1, const matrix33& operand2);


#endif
