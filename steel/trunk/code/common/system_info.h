/*id*********************************************************
	Unit: System Info
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
        Steel Engine License
    Description:
		Модуль определения необходимых параметров системы.
 ************************************************************/
#ifndef COMMON__SYSTEM_INFO_H
#define COMMON__SYSTEM_INFO_H

#include "../steel.h"
#if STEEL_OS == OS_WIN32
void CollectSystemInfo(void);
#else
#define CollectSystemInfo() ;
#endif

#endif
