/****************************************************************
  TheGame 2D & 3D geometry unit (Steel engine)
  (C) KindeX (Andrey Ivanov), kindex@inbox.lv, http://kindex.lv
  Division, 2004-2005.

  Vse razmeti v igre v sisteme CI:
  kilogramm, metr, sekunda, m/s, m/ss, dzhoul', Njuton i t.d.
  ili v % (coord 0..1)
******************************************************************/

#include "geometry.h"
#include "maths.h"

bool intersect(float amin, float amax, float bmin, float bmax)
{
	if(bmin > amin) amin = bmin;
	if(bmax < amax) amax = bmax;

    return
		amin  < amax+ EPSILON;
}



