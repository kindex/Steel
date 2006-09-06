/*id*********************************************************
	File: graph/opengl/ext/normalisation_cube_map.cpp
	Unit: opengl ext
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Normalisation cube map extension
 ************************************************************/
#ifndef __OPENGL_NORMALISATION_CUBE_MAP
#define __OPENGL_NORMALISATION_CUBE_MAP

#include "../../../steel.h"

#ifdef  _WIN32
	#include    <windows.h>
#else
	#define GLX_GLXEXT_LEGACY
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "../gl/libext.h"

GLuint generateNormalisationCubeMap(void);

#endif
