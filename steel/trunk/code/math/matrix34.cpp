/*id*********************************************************
    Unit: math/matrix4x4
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

void matrix34::loadIdentity(void)
{
	data.matrix.loadIdentity();
	data.vector.x = data.vector.y = data.vector.z = 0.0f;
}

void matrix34::loadZero(void)
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

void matrix34::operator*=(const matrix34 operand)
{
	data.matrix *= operand.data.matrix;
	data.vector += data.matrix * operand.data.vector;
}

