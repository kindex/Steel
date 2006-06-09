/*id*********************************************************
    Unit: math/matrix4x4
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Description:
		Class declaration for a 3x4 matrix
		3 Rows x 4 Columns
		| 0 1 2 x |
		| 3 4 5 y |
		| 6 7 8 z |
		| 0 0 0 1 |
************************************************************/

#ifndef __MATRIX34_H
#define __MATRIX34_H

#include "vector3d.h"
#include "matrix33.h"


struct matrix34
{
	//member variables
	union
	{
		float a[12]; // 1D array
		float m[4][3]; // 2D array - matrix 4 rows x 3 cols
		struct
		{
			matrix33 matrix;
			v3simple vector;
		} combine;
	} data;

	void loadIdentity(void);
	void loadZero(void);

	static const inline matrix34 get(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22,	float _30, float _31, float _32)
	{
		matrix34 t;
		t.data.m[0][0] = _00;	t.data.m[0][1] = _01;	t.data.m[0][2] = _02;
		t.data.m[1][0] = _10;	t.data.m[1][1] = _11;	t.data.m[1][2] = _12;
		t.data.m[2][0] = _20;	t.data.m[2][1] = _21;	t.data.m[2][2] = _22;
		t.data.m[3][0] = _30;	t.data.m[3][1] = _31;	t.data.m[3][2] = _32;
		return t;
	}

	inline static const matrix34	getIdentity() 
	{ 
		return get(1,0,0, 0,1,0, 0,0,1, 0,0,0); 
	}


	//binary operators
	void operator+=(const matrix34 operand);
	inline matrix34 operator+(const matrix34 operand) const	{ matrix34 m = *this; m += operand; return m; }

	void operator-=(const matrix34 operand);
	inline matrix34 operator-(const matrix34 operand) const	{ matrix34 m = *this; m -= operand; return m; }

	void operator*=(const float operand);
	inline matrix34 operator*(const float operand) const	{ matrix34 m = *this; m *= operand; return m; }

	void operator/=(const float operand);
	inline matrix34 operator/(const float operand) const	{ matrix34 m = *this; m /= operand; return m; }

	inline void setRotationX(const float angle) { data.combine.matrix.setRotationX(angle); }
	inline void setRotationX(const float sin, const float cos) { data.combine.matrix.setRotationX(sin, cos); }
	inline void setRotationY(const float angle) { data.combine.matrix.setRotationY(angle); }
	inline void setRotationY(const float sin, const float cos) { data.combine.matrix.setRotationY(sin, cos); }
	inline void setRotationZ(const float angle) { data.combine.matrix.setRotationZ(angle); }
	inline void setRotationZ(const float sin, const float cos) { data.combine.matrix.setRotationZ(sin, cos); }
	inline void setScale(const v3 scale) {data.combine.matrix.setScale(scale);}

	v3 getRow(const int rowNumber) const
	{
		v3 temp;
		temp.x = data.m[rowNumber][0];
		temp.y = data.m[rowNumber][1];
		temp.z = data.m[rowNumber][2];
		return temp;
	}

	void setRow(int rowNumber, v3 row)
	{
		data.m[rowNumber][0] = row.x;
		data.m[rowNumber][1] = row.y;
		data.m[rowNumber][2] = row.z;
	}

	inline void setTranslation(v3 row) 	{		setRow(3, row);	}
	inline v3 getTranslation(void) const 	{	return getRow(3);	}

	void operator*=(const matrix34 operand);
	inline matrix34 operator*(const matrix34 operand) const	{ matrix34 m = *this; m *= operand; return m; }

	inline v3 operator*(const v3 operand) const	
	{ 
		return  this->data.combine.matrix*operand + v3(data.combine.vector);
	}
	matrix33 &getMatrix33(void) { return data.combine.matrix; }
	
	v3 getVector(void) { return data.combine.vector; }

	inline void setRotationAxis(const float angle, const v3 axis)	{		setRotationAxis(sin(angle), sin(angle), axis);	}
	void setRotationAxis(const float sinAngle, const float cosAngle, const v3 axis)
	{
		data.combine.matrix.setRotationAxis(sinAngle, cosAngle, axis);
		data.combine.vector.loadZero();
	}

	inline matrix34 getInverse(void) const
	{
		matrix34 temp;
		temp.data.combine.matrix = data.combine.matrix.getInverse();
		temp.data.combine.vector = -data.combine.vector;
		return temp;
	}


};


#endif
