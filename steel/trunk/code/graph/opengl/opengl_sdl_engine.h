/*id*********************************************************
	File: graph/opengl/opengl_sdl_engine.h
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		Часть графического движока OpenGL, которая реализует
		системно зависимые функции, такие как создание окна.
	TODO
		delete class
************************************************************/

#ifndef __GRAPH_OPENGL_SDL_ENGINE_H
#define __GRAPH_OPENGL_SDL_ENGINE_H

#include "../../steel.h"
#include "../../_cpp.h"

#if STEEL_OPENGL_API == OPENGL_SDL

#include "opengl_engine.h"
#include "../../common/logger.h"
#include "SDL.h"			// Header File For SDL


class OpenGL_SDL_Engine: public OpenGL_Engine
{
protected:
	SDL_Surface *surface;	// SDL surface

	void swapBuffers();
	bool createWindow();

public:
	OpenGL_SDL_Engine(void) { surface = NULL;}
	bool deinit(void);
	void setCaption(std::string caption);
};
#endif
#endif
