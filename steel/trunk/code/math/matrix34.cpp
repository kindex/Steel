/*id*********************************************************
    Unit: math/matrix4x4
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Description:
		Class declaration for a 3x4 matrix
************************************************************/

#include "matrix34.h"
#include <memory.h>

void matrix34::loadIdentity(void)
{
	memset(data.a, 0, 12*sizeof(float));
	data.m[0][0] = 1.0f;
	data.m[1][1] = 1.0f;
	data.m[2][2] = 1.0f;
}

void matrix34::loadZero(void)
{
	memset(data.a, 0, 12*sizeof(float));
}

void matrix34::operator+=(const matrix34 operand)
{
	data.combine.matrix += operand.data.combine.matrix;
	data.combine.vector += operand.data.combine.vector;
}

void matrix34::operator-=(const matrix34 operand)
{
	data.combine.matrix -= operand.data.combine.matrix;
	data.combine.vector -= operand.data.combine.vector;
}

void matrix34::operator*=(const float operand)
{
	data.combine.matrix *= operand;
	data.combine.vector *= operand;
}

void matrix34::operator/=(const float operand)
{
	data.combine.matrix *= operand;
	data.combine.vector *= operand;
}

void matrix34::operator*=(const matrix34 operand)
{
	data.combine.matrix *= operand.data.combine.matrix;
	data.combine.vector += data.combine.matrix * operand.data.combine.vector;
}

