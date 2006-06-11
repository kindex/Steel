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
	data.combine.matrix.loadIdentity();
	data.combine.vector.x = data.combine.vector.y = data.combine.vector.z = 0.0f;
}

void matrix34::loadZero(void)
{
	data.combine.matrix.loadZero();
	data.combine.vector.x = data.combine.vector.y = data.combine.vector.z = 0.0f;
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

