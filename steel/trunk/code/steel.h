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

//	Компиляторы
#define GCC 0
#define VS8 1

//	ОС
#define OS_LINUX 0
#define OS_WIN32 1

//	Видео подсистемы
#define OPENGL_SDL 0
#define OPENGL_WIN 1

//	Определение компилятора
#if defined(__GNUC__)
	#if !defined(STEEL_COMPILLER) || (STEEL_COMPILER != GCC)
		#undef STEEL_COMPILER
		#define STEEL_COMPILER GCC
		
		//	для совместимости
		#ifndef STEEL_COMPILER_DEVCPP
			#define STEEL_COMPILER_DEVCPP
		#endif	// STEEL_COMPILER_DEVCPP
		
		//	для совместимости
		#undef STEEL_COMPILER_VS8
	#endif	// STEEL_COMPILER
	
#else	// __GNUC__
	#if !defined(STEEL_COMPILER) || (STEEL_COMPILER != VS8)
		#undef STEEL_COMPILER
		#define STEEL_COMPILER VS8
		
		//	для совместимости
		#ifndef STEEL_COMPILER_VS8
			#define STEEL_COMPILER_VS8
		#endif	// STEEL_COMPILER_VS8
		
		//	для совместимости
		#undef STEEL_COMPILER_DEVCPP
	#endif	// STEEL_COMPILER
#endif	// __GNUC__

//	Компиляция происходит под линуксом
#if defined(__linux)
	#if !defined(STEEL_VIDEOSYS) || (STEEL_VIDEOSYS != OPENGL_SDL)
		#undef STEEL_VIDEOSYS
		#define STEEL_VIDEOSYS OPENGL_SDL

		//	для совместимости
		#ifndef STEEL_OPENGL_SDL
			#define STEEL_OPENGL_SDL
		#endif	// STEEL_OPENGL_SDL

		//	для совместимости
		#undef STEEL_OPENGL_WIN
	#endif	// STEEL_VIDEOSYS
	
	#if !defined(STEEL_OS) || (STEEL_OS != OS_LINUX)
		#undef STEEL_OS
		#define STEEL_OS OS_LINUX
		
		//	для совместимости
		#ifdef STEEL_OS_WIN32
			#undef STEEL_OS_WIN32
		#endif	// STEEL_OS_WIN32
	#endif	// STEEL_OS
	
#elif defined(WIN32)
	#if !defined(STEEL_VIDEOSYS)
		#define STEEL_VIDEOSYS OPENGL_WIN
	#endif	// STEEL_VIDEOSYS
	
	//	для совместимости
	#ifndef STEEL_OPENGL_WIN
		#define STEEL_OPENGL_WIN
	#endif	// STEEL_OPENGL_WIN
	
	//	для совместимости
	#undef STEEL_OPENGL_SDL
	
#else	// __linux, WIN32
	#error Cannot determine the target OS.
#endif	// __linux, WIN32

/*
	Абстрактный класс, от которого наследуются все классы в проекте
*/
class steelAbstract
{
public:
	steelAbstract( void ) {}
	virtual ~steelAbstract( void ) {}
	steelError_t getError( void ) { return lastError; }
protected:
	steelError_t lastError;
};

#endif
