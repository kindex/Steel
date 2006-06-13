/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
    License:
        Steel Engine License
    Description:
		ќсновные директивы и определени€ (includes) дл€ всего проекта
 ************************************************************/


#ifndef _CPP_H
#define _CPP_H

// OS
#define STEEL_WIN32

//#define OPENGL_SDL
#define OPENGL_WIN

//#define Linux

//Compiler
//#undef BC // Borland C++ Builder 6 
//--define COMPILER_VS8 // Visual Studio 8 (2005) - automatic in steel.sln

#ifndef COMPILER_VS8
#define COMPILER_DEVCPP
#endif

#define _CRT_SECURE_NO_DEPRECATE 

#include "common/logger.h"
#include "common/types.h"

using namespace steel;

#endif
