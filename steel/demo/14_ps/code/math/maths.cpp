#include "maths.h"

float clamp(float a)
{
	if(a<0) return 0;
	else if(a>1) return 1;
	else return a;
}

