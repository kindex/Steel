/*id*********************************************************
	File: math/matrix33.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Class declaration for a 3x3 matrix
************************************************************/

#include "matrix33.h"
#include <memory.h>

/*matrix33::matrix33(const float _00,	const float _01,	const float _02,	const float _10,	const float _11,	const float _12,	const float _20,	const float _21,	const float _22)
{
	data.m[0][0] = _00;
	data.m[0][1] = _01;
	data.m[0][2] = _02;
	data.m[1][0] = _10;
	data.m[1][1] = _11;
	data.m[1][2] = _12;
	data.m[2][0] = _20;
	data.m[2][1] = _21;
	data.m[2][2] = _22;
}*/

/*
template<typename T>void matrix33<T>::operator+=(const matrix33 operand)
{
	data.a[0] += operand.data.a[0];
	data.a[1] += operand.data.a[1];
	data.a[2] += operand.data.a[2];
	data.a[3] += operand.data.a[3];
	data.a[4] += operand.data.a[4];
	data.a[5] += operand.data.a[5];
	data.a[6] += operand.data.a[6];
	data.a[7] += operand.data.a[7];
	data.a[8] += operand.data.a[8];
}*/



void matrix33::operator+=(const matrix33 operand)
{
	data.a[0] += operand.data.a[0];
	data.a[1] += operand.data.a[1];
	data.a[2] += operand.data.a[2];
	data.a[3] += operand.data.a[3];
	data.a[4] += operand.data.a[4];
	data.a[5] += operand.data.a[5];
	data.a[6] += operand.data.a[6];
	data.a[7] += operand.data.a[7];
	data.a[8] += operand.data.a[8];
}


void matrix33::loadIdentity()
{
	data.m[0][0] = 1;	data.m[0][1] = 0;	data.m[0][2] = 0;
	data.m[1][0] = 0;	data.m[1][1] = 1;	data.m[1][2] = 0;
	data.m[2][0] = 0;	data.m[2][1] = 0;	data.m[2][2] = 1;
}

void matrix33::operator-=(const matrix33 operand)
{
	data.a[0] -= operand.data.a[0];
	data.a[1] -= operand.data.a[1];
	data.a[2] -= operand.data.a[2];
	data.a[3] -= operand.data.a[3];
	data.a[4] -= operand.data.a[4];
	data.a[5] -= operand.data.a[5];
	data.a[6] -= operand.data.a[6];
	data.a[7] -= operand.data.a[7];
	data.a[8] -= operand.data.a[8];
}

void matrix33::operator*=(const float operand)
{
	data.a[0] *= operand;
	data.a[1] *= operand;
	data.a[2] *= operand;
	data.a[3] *= operand;
	data.a[4] *= operand;
	data.a[5] *= operand;
	data.a[6] *= operand;
	data.a[7] *= operand;
	data.a[8] *= operand;
}

void matrix33::operator /=(const float operand)
{
	data.a[0] /= operand;
	data.a[1] /= operand;
	data.a[2] /= operand;
	data.a[3] /= operand;
	data.a[4] /= operand;
	data.a[5] /= operand;
	data.a[6] /= operand;
	data.a[7] /= operand;
	data.a[8] /= operand;
}

void matrix33::loadZero()
{
	memset(data.a, 0, 12*sizeof(float));
}


void matrix33::operator*=(const matrix33 o)
{
	matrix33 copy = *this; // copy
	const m33 &a = copy.data.m;
	const m33 &b = o.data.m;

	data.m[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
	data.m[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
	data.m[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];

	data.m[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
	data.m[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
	data.m[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];

	data.m[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
	data.m[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
	data.m[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
}

void matrix33::setRotationZ(const float sin, const float cos)
{
	data.m[0][0] = cos;		data.m[0][1] = -sin;	data.m[0][2] = 0;
	data.m[1][0] = sin;		data.m[1][1] = cos;		data.m[1][2] = 0;
	data.m[2][0] = 0;		data.m[2][1] = 0;		data.m[2][2] = 1;
}

void matrix33::setRotationX(const float sin, const float cos)
{
	data.m[0][0] = 1;		data.m[0][1] = 0;	data.m[0][2] = 0;
	data.m[1][0] = 0;		data.m[1][1] = cos;	data.m[1][2] = -sin;
	data.m[2][0] = 0;		data.m[2][1] = sin;data.m[2][2] = cos;
}

void matrix33::setRotationY(const float sin, const float cos)
{
	data.m[0][0] = cos;		data.m[0][1] = 0;	data.m[0][2] = sin;
	data.m[1][0] = 0;		data.m[1][1] = 1;	data.m[1][2] = 0;
	data.m[2][0] = -sin;	data.m[2][1] = 0;	data.m[2][2] = cos;
}

void matrix33::setScale(const v3 scale)
{
	data.m[0][0] = scale.x;	data.m[0][1] = 0;		data.m[0][2] = 0;
	data.m[1][0] = 0;		data.m[1][1] = scale.y;	data.m[1][2] = 0;
	data.m[2][0] = 0;		data.m[2][1] = 0;		data.m[2][2] = scale.z;
}


void operator*=(v3 &operand1, const matrix33 operand2)
{
	v3 copy = operand1;
	operand1.x = copy.x*operand2.data.m[0][0] + copy.y*operand2.data.m[1][0] + copy.z*operand2.data.m[2][0];
	operand1.y = copy.x*operand2.data.m[0][1] + copy.y*operand2.data.m[1][1] + copy.z*operand2.data.m[2][1];
	operand1.z = copy.x*operand2.data.m[0][2] + copy.y*operand2.data.m[1][2] + copy.z*operand2.data.m[2][2];
}

v3 matrix33::operator*(const v3 operand) const
{
	v3 result;
	result.x = operand.x*data.m[0][0] + operand.y*data.m[0][1] + operand.z*data.m[0][2];
	result.y = operand.x*data.m[1][0] + operand.y*data.m[1][1] + operand.z*data.m[1][2];
	result.z = operand.x*data.m[2][0] + operand.y*data.m[2][1] + operand.z*data.m[2][2];
	return result;
}

void matrix33::setRotationAxis(const float s, const float c, const v3 v)
{
	data.m[0][0] = v.x*v.x*(1-c) + c;
	data.m[0][1] = v.x*v.y*(1-c) - s*v.z;
	data.m[0][2] = v.x*v.z*(1-c) + s*v.y;

	data.m[1][0] = v.x*v.y*(1-c) + s*v.z;
	data.m[1][1] = v.y*v.y*(1-c) + c;
	data.m[1][2] = v.y*v.z*(1-c) - s*v.x;

	data.m[2][0] = v.x*v.z*(1-c) - s*v.y;
	data.m[2][1] = v.y*v.z*(1-c) + s*v.x;
	data.m[2][2] = v.z*v.z*(1-c) + c;
}

void matrix33::getInverse(matrix33 &res) const
{
	const m33 &m = data.m;

	float det = 
		+ m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1])
		+ m[0][1]*(m[1][2]*m[2][0] - m[1][0]*m[2][2])
		+ m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);


	if(det != 0)
	{
		// алгебраические дополнения
		float m00 =  m[1][1]*m[2][2] - m[1][2]*m[2][1];
		float m01 = -m[0][1]*m[2][2] + m[2][1]*m[0][2];
		float m02 =  m[0][1]*m[1][2] - m[1][1]*m[0][2];

		float m10 = -m[1][0]*m[2][2] + m[2][0]*m[1][2];
		float m11 =  m[0][0]*m[2][2] - m[2][0]*m[0][2];
		float m12 = -m[0][0]*m[1][2] + m[1][0]*m[0][2];

		float m20 =  m[1][0]*m[2][1] - m[2][0]*m[1][1];
		float m21 = -m[0][0]*m[2][1] + m[0][1]*m[2][0];
		float m22 =  m[0][0]*m[1][1] - m[1][0]*m[0][1];

		float d = 1/det;
		res.data.m[0][0] = m00*d;
		res.data.m[0][1] = m01*d;
		res.data.m[0][2] = m02*d;

		res.data.m[1][0] = m10*d;
		res.data.m[1][1] = m11*d;
		res.data.m[1][2] = m12*d;

		res.data.m[2][0] = m20*d;
		res.data.m[2][1] = m21*d;
		res.data.m[2][2] = m22*d;
	}
	else
		res.loadZero();

}
