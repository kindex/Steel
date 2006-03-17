/***************************************************************************
 *            types.h
 *
 *  Fri Feb 17 10:29:46 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __TYPES_H__
#define __TYPES_H__

/*
	Промежуток времени (секунды)
*/
typedef
double steelTime_t;

/*
	Промежуток времени (миллисекунды)
*/
typedef
unsigned int steelTimeM_t;

/*
	Путь (метры)
*/
typedef
double steelLength_t;

/*
	Кадры
*/
typedef
unsigned int steelFrame_t;

/*
	Поле флагов
*/
typedef
unsigned long int steelFlag_t;

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
	SE_SDL_INIT		//	ошибка инициализации библиотеки SDL
}	steelError_t;

#endif
