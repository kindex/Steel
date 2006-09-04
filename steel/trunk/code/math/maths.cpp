/*id*********************************************************
	File: math/maths.cpp
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Полезные математические функции функции
 *****************************************************************/

#include "maths.h"

float clamp(float a)
{
	if(a<0) return 0;
	else if(a>1) return 1;
	else return a;
}

