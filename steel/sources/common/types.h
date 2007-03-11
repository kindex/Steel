/*id*********************************************************
	File: common/timer.h
	Unit: Timer
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		Базовые типы
 ************************************************************/

#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__

#include "../steel.h"
#include "pvector.h"

/*
	Целочисленные типы данных
*/
typedef unsigned int uint;

#if STEEL_OS==OS_LINUX
#include <sys/types.h>
typedef int8_t int8;
typedef u_int8_t uint8;
typedef int16_t int16;
typedef u_int16_t uint16;
typedef int32_t int32;
typedef u_int32_t uint32;
typedef int64_t int64;
typedef u_int64_t uint64;

#else
//	только для 32 бит!!
typedef signed char int8;
typedef unsigned char uint8;
typedef short int int16;
typedef unsigned short int uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int int64;
typedef unsigned long long int uint64;
#endif

/*
	Промежуток времени (секунды)
*/
namespace steel
{
typedef float time; // time interval
}

typedef steel::pvector<int> IntVector;
typedef steel::pvector<float> FloatVector;

#endif
