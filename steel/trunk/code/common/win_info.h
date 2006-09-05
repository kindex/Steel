/*id*********************************************************
	File: common/win_info.h
	Unit: Windows Info
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Модуль определения версии Windows.
 ************************************************************/
#ifndef COMMON__WIN_INFO_H
#define COMMON__WIN_INFO_H

#include "../steel.h"
#if STEEL_OS == OS_WIN32
	void DetectWindowsVersion(void);
#endif

#endif
