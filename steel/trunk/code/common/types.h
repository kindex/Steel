/***************************************************************************
 *            types.h
 *
 *  Fri Feb 17 10:29:46 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __TYPES_H__
#define __TYPES_H__

#if STEEL_OS == OS_LINUX
#include <sys/types.h>
#endif

/*
	Промежуток времени (секунды)
*/
namespace steel
{
typedef float time; // time interval
}

typedef float coord;
typedef float steelLength_t;

/*
	Промежуток времени (миллисекунды)
*/
typedef
unsigned int steelTimeM_t;

/*
	Кадры
*/
typedef
unsigned int steelFrame_t;
/*
	Поле флагов (32 бита)
*/
typedef
unsigned int steelFlag32_t;

/*
	Угол (0 = 0 градусов, 2^32-1 -> 360-0 градусам)
*/ 
typedef
unsigned long int steelAngle_t;

/*
	Коды ошибок
*/
typedef
enum
{
	SE_NONE,		//	нет ошибки
	SE_NOMEM,		//	ошибка памяти
	SE_LOADED,		//	объект уже загружен
	SE_NOT_LOADED,	//	объект ещё не загружен
	SE_INV_FMT,		//	неверный формат
	SE_SDL_VQUERY,	//	неудачный запрос к SDL
	SE_SDL_VIDEO,	//	не удалось создать видео режим
	SE_SDL_INIT,	//	ошибка инициализации библиотеки SDL
	SE_OGL_INIT		//	ошибка инициализации OpenGL
}	SteelError;

#endif
