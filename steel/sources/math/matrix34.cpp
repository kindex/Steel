/*id*********************************************************
	File: math/matrix34.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Class declaration for a 3x4 matrix
************************************************************/

#include "matrix34.h"
#include <memory.h>

void matrix34::loadIdentity()
{
	data.matrix.loadIdentity();
	data.vector.x = data.vector.y = data.vector.z = 0.0f;
}

void matrix34::loadZero()
{
	data.matrix.loadZero();
	data.vector.x = data.vector.y = data.vector.z = 0.0f;
}

void matrix34::operator+=(const matrix34 operand)
{
	data.matrix += operand.data.matrix;
	data.vector += operand.data.vector;
}

void matrix34::operator-=(const matrix34 operand)
{
	data.matrix -= operand.data.matrix;
	data.vector -= operand.data.vector;
}

void matrix34::operator*=(const float operand)
{
	data.matrix *= operand;
	data.vector *= operand;
}

void matrix34::operator/=(const float operand)
{
	data.matrix *= operand;
	data.vector *= operand;
}

void matrix34::operator*=(const matrix34 b)
{
	matrix34 a = *this; // copy

#define A a.data.matrix.data.m
#define B b.data.matrix.data.m
#define C this->data.matrix.data.m

	C[0][0] = A[0][0]*B[0][0] + A[1][0]*B[0][1] + A[2][0]*B[0][2];
	C[0][1] = A[0][1]*B[0][0] + A[1][1]*B[0][1] + A[2][1]*B[0][2];
	C[0][2] = A[0][2]*B[0][0] + A[1][2]*B[0][1] + A[2][2]*B[0][2];

	C[1][0] = A[0][0]*B[1][0] + A[1][0]*B[1][1] + A[2][0]*B[1][2];
	C[1][1] = A[0][1]*B[1][0] + A[1][1]*B[1][1] + A[2][1]*B[1][2];
	C[1][2] = A[0][2]*B[1][0] + A[1][2]*B[1][1] + A[2][2]*B[1][2];

	C[2][0] = A[0][0]*B[2][0] + A[1][0]*B[2][1] + A[2][0]*B[2][2];
	C[2][1] = A[0][1]*B[2][0] + A[1][1]*B[2][1] + A[2][1]*B[2][2];
	C[2][2] = A[0][2]*B[2][0] + A[1][2]*B[2][1] + A[2][2]*B[2][2];

	data.vector.x = a.data.vector.x*B[0][0] + a.data.vector.y*B[0][1] + a.data.vector.z*B[0][2] + b.data.vector.x;
	data.vector.y = a.data.vector.x*B[1][0] + a.data.vector.y*B[1][1] + a.data.vector.z*B[1][2] + b.data.vector.y;
	data.vector.z = a.data.vector.x*B[2][0] + a.data.vector.y*B[2][1] + a.data.vector.z*B[2][2] + b.data.vector.z;
#undef A
#undef B
#undef C
}

v3 operator*(IN const v3& operand1, IN const matrix34& operand2)
{
	v3 copy = operand1;
	copy *= operand2;
	return copy;
}

void operator*=(IN OUT v3& operand1, IN const matrix34& operand2)
{
	v3 copy = operand1;
#define B operand2.data.matrix.data.m
#define V operand2.data.vector
	
	operand1.x = operand1.x*B[0][0] + operand1.y*B[0][1] + operand1.z*B[0][2] + V.x;
	operand1.y = operand1.x*B[1][0] + operand1.y*B[1][1] + operand1.z*B[1][2] + V.y;
	operand1.z = operand1.x*B[2][0] + operand1.y*B[2][1] + operand1.z*B[2][2] + V.z;

#undef B
#undef V
}
