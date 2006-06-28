/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine SDL
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
 ************************************************************/

#ifndef __GRAPH_OPENGL_SDL_ENGINE_H
#define __GRAPH_OPENGL_SDL_ENGINE_H

#include "../../_cpp.h"

#ifdef STEEL_OPENGL_SDL

#include "opengl_engine.h"
#include "../../common/logger.h"
#include <SDL.h>			// Header File For SDL


class OpenGL_SDL_Engine: public OpenGL_Engine
{
protected:
	SDL_Surface *surface;	// SDL surface

	void swapBuffers();
	bool createWindow();

public:
	OpenGL_SDL_Engine() { surface = NULL;}
	bool deinit();
	void setCaption(std::string caption);
};
#endif
#endif
