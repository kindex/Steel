/*id*********************************************************
	File: math/matrix34.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
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
	struct
	{
		matrix33 matrix; // first index - row number, second - column number
		v3simple vector;
	} data;

	void loadIdentity();
	void loadZero();

	static const inline matrix34 get(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22,	float _x, float _y, float _z)
	{
		matrix34 t;
		t.data.matrix.data.m[0][0] = _00;	t.data.matrix.data.m[0][1] = _01;	t.data.matrix.data.m[0][2] = _02;
		t.data.matrix.data.m[1][0] = _10;	t.data.matrix.data.m[1][1] = _11;	t.data.matrix.data.m[1][2] = _12;
		t.data.matrix.data.m[2][0] = _20;	t.data.matrix.data.m[2][1] = _21;	t.data.matrix.data.m[2][2] = _22;

		t.data.vector.x = _x;		t.data.vector.y = _y;		t.data.vector.z = _z;
		return t;
	}

	inline static const matrix34	getIdentity()
	{ 
		return get(1,0,0, 0,1,0, 0,0,1, 0,0,0); 
	}

	inline static const matrix34	CreateTranslationMatrix(const v3 translation)
	{ 
		return get(1,0,0, 0,1,0, 0,0,1, translation.x, translation.y, translation.z); 
	}
	inline static const matrix34	CreateRotationMatrix(const float angle, const v3 rotation)
	{ 
		matrix34 matrix;
		matrix.setRotationAxis(angle, rotation);
		return matrix; 
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

	inline void setRotationX(const float angle) { data.matrix.setRotationX(angle);  data.vector.loadZero();}
	inline void setRotationX(const float sin, const float cos) { data.matrix.setRotationX(sin, cos);  data.vector.loadZero();}
	inline void setRotationY(const float angle) { data.matrix.setRotationY(angle);  data.vector.loadZero();}
	inline void setRotationY(const float sin, const float cos) { data.matrix.setRotationY(sin, cos);  data.vector.loadZero();}
	inline void setRotationZ(const float angle) { 		data.matrix.setRotationZ(angle); data.vector.loadZero();	}
	inline void setRotationZ(const float sin, const float cos) { data.matrix.setRotationZ(sin, cos);  data.vector.loadZero();}
	inline void setScale(const v3 scale) {data.matrix.setScale(scale);  data.vector.loadZero();}

	inline v3 getScale() const { return data.matrix.getScale(); }

	v3 getRow(const int rowNumber) const
	{
		v3 temp;
		temp.x = data.matrix.data.m[rowNumber][0];
		temp.y = data.matrix.data.m[rowNumber][1];
		temp.z = data.matrix.data.m[rowNumber][2];
		return temp;
	}

	void setRow(int rowNumber, v3 row)
	{
		data.matrix.data.m[rowNumber][0] = row.x;
		data.matrix.data.m[rowNumber][1] = row.y;
		data.matrix.data.m[rowNumber][2] = row.z;
	}

	inline void setTranslation(const v3& row)
	{	
		data.vector.x = row.x;
		data.vector.y = row.y;
		data.vector.z = row.z;
	}
	inline v3 getTranslation() const 	{	return v3(data.vector.x, data.vector.y, data.vector.z);	}

	void operator*=(const matrix34 operand);
	inline matrix34 operator*(const matrix34 operand) const	{ matrix34 m = *this; m *= operand; return m; }

	inline v3 operator*(const v3 operand) const	
	{ 
		return  this->data.matrix*operand + v3(data.vector);
	}
	matrix33& getMatrix33() { return data.matrix; }
	
	v3 getVector() { return data.vector; }

	inline void setRotationAxis(const float angle, const v3 axis)	{		setRotationAxis(sin(angle), cos(angle), axis);	}
	void setRotationAxis(const float sinAngle, const float cosAngle, const v3 axis)
	{
		data.matrix.setRotationAxis(sinAngle, cosAngle, axis);
		data.vector.loadZero();
	}

	inline void getInverse(matrix34 &operand) const
	{
		data.matrix.getInverse(operand.data.matrix);
		operand.data.vector = -data.vector;
	}

	inline matrix34 getInverse() const
	{
		matrix34 temp;
		getInverse(temp);
		return temp;
	}
};

v3 operator*(IN const v3& operand1, IN const matrix34& operand2);
void operator*=(IN OUT v3& operand1, IN const matrix34& operand2);

#endif
