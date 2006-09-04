/*id*********************************************************
	File: steel.h
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		Основные директивы и определения (includes) для всего проекта.
		Часть директив хранится в makefiles и аналогичных
 ************************************************************/

#ifndef __STEEL_H__
#define __STEEL_H__

#include "common/types.h"

//	Компиляторы (STEEL_COMPILER)
#define COMPILER_GCC 0
#define COMPILER_VS8 1
#define COMPILER_DEVCPP 2

//	ОС (STEEL_OS)
#define OS_LINUX 0
#define OS_WIN32 1

//	Видео подсистемы (STEEL_OPENGL_API)
#define OPENGL_SDL 0
#define OPENGL_WINAPI 1

/*
	STEEL_VECTOR определена, когда svector использует реализацию 
	steel::vector (common/steel_vector.h),
	иначе использует std::vector
*/
#define STEEL_VECTOR


/*
	Абстрактный класс, от которого наследуются все классы в проекте
*/
/*class SteelAbstract
{
public:
	SteelAbstract( void ) {}
	virtual ~SteelAbstract( void ) {}
	SteelError getError( void ) { return lastError; }
	
protected:
	void setError( SteelError errorCode ) { lastError = errorCode; }
	
private:
	SteelError lastError;
};
*/
#define steelPushError( errorCode ) \
{ \
	setError(errorCode); \
	return false; \
}

#ifdef STEEL_USE_GETTEXT
	#define _( msgid ) gettext(msgid)
#else
	#define _( msgid )
#endif

#endif
