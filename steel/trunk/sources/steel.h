/*id*********************************************************
	File: steel.h
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
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
#define COMPILER_GCC 2
#define COMPILER_VS8 1

//	ОС (STEEL_OS)
#define OS_LINUX 2
#define OS_WIN32 1

#if !defined(STEEL_COMPILER)
	#error STEEL_COMPILER is not defined
#endif

#if !defined(STEEL_OS)
	#error STEEL_OS
#endif


//#define LIB_SDL

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

#ifndef NULL
	#define NULL 0
#endif

#define EACH(CLASS, OBJECT, IT) (CLASS::iterator IT = (OBJECT).begin(); IT != (OBJECT).end(); IT++)
#define EACH_CONST(CLASS, OBJECT, IT) (CLASS::const_iterator IT = (OBJECT).begin(); IT != (OBJECT).end(); IT++)
#define EVER (;;)

#endif
