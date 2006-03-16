/***************************************************************************
 *            steel.h
 *
 *  Sat Mar  4 11:17:44 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __STEEL_H__
#define __STEEL_H__

#include "common/types.h"

/*
	Абстрактный класс, от которого наследуются все классы в проекте
*/
class steelAbstract
{
public:
	steelError_t getError( void ) { return lastError; }
protected:
	steelError_t lastError;
};

#endif
