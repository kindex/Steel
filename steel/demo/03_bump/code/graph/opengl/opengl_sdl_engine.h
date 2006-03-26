#ifndef __GRAPH_OPENGL_SDL_ENGINE_H
#define __GRAPH_OPENGL_SDL_ENGINE_H

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