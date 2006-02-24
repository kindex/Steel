/***************************************************************************
 *            simple.h
 *
 *  Fri Feb 24 10:17:40 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __BONES_SIMPLE_H__
#define __BONES_SIMPLE_H__

#include "../bones.h"

namespace boneSpace
{
	boneErr simple_load( const std::string file,
			boneSet *&set, boneAnim *&anim, unsigned int &animc, steelTimeM_t &time);
}

#endif
