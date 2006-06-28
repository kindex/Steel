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
		ќсновные директивы и определени€ (includes) дл€ всего проекта.
		„асть директив хранитс€ в makefiles и аналогичных
 ************************************************************/

#ifndef _CPP_H
#define _CPP_H

/*
ƒирективы:
STEEL_OS_WIN32 Ц определена, если проект компилируетс€ дл€ WIN32 платформы
STEEL_OPENGL_WIN Ц определена, если проект использует создание окна средствами Windows
STEEL_OPENGL_SDL Ц определена, если проект использует создание окна средствами SDL
STEEL_COMPILER_VS8 Ц определена, если проект компилируетс€ компил€тором Microsoft Visual Studio 8 (2005)
STEEL_COMPILER_DEVCPP Ц определена, если проект компилируетс€ компил€тором Dev-C++
*/

#include "common/logger.h"
#include "common/types.h"
#include "common/utils.h"

using namespace steel;

#endif
