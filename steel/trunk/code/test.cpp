#include "_cpp.h"
#include "common/logger.h"

#include "math/line.h"
#include "math/plane.h"

//#include <assert.h>

//#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )


bool test()
{
	v3 a(1,1,1), b(2,2,2), c(1,-1,0);

	v3 d = a + b*c;


	return true;
}
