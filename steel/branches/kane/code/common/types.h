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
	Поле флагов (32 бита)
*/
typedef
uint32_t steelFlag32_t;

/*
	Угол (0 = 0 градусов, 2^32-1 -> 360-0 градусам)
*/
typedef
uint32_t steelAngle_t;

#endif
