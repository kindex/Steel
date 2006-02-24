#include "maths.h"

template<typename T> T sqr(T x) 
{
	return x*x;
}

template<typename T>int sgn(T A) 
{ 
	return (A>EPSILON)-(A<-EPSILON);
}
