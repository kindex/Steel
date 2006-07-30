/*id*********************************************************
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

#ifndef _CPP_H
#define _CPP_H

/*
Директивы:
STEEL_OS_WIN32 – определена, если проект компилируется для WIN32 платформы
STEEL_OPENGL_WIN – определена, если проект использует создание окна средствами Windows
STEEL_OPENGL_SDL – определена, если проект использует создание окна средствами SDL
STEEL_COMPILER_VS8 – определена, если проект компилируется компилятором Microsoft Visual Studio 8 (2005)
STEEL_COMPILER_DEVCPP – определена, если проект компилируется компилятором Dev-C++
*/

#include "common/logger.h"
#include "common/types.h"
#include "common/utils.h"

using namespace steel;

#endif
