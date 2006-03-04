#ifndef __GRAPH_OPENGL_ENGINE_H
#define __GRAPH_OPENGL_ENGINE_H

#include <string>
#include "SDL.h"			// Header File For SDL

#include "graph_engine.h"
#ifdef __linux
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <GL/glaux.h>		// Header File For The Glaux Library
#else
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#endif

struct Window: public steelAbstract
{
    string caption, title; // if not fullscreen
	SDL_Surface *surface;	// SDL surface
    int width, height, bpp;
	bool needupdate; // true on resize

	Window();
    bool createWin();
    void create();
    void repair();
};



class OpenGL_Engine: public GraphEngine
{
//protected:
//	void clearWindow();

public:
	bool init();
	bool process();
	bool deinit();

	Window *window;
};



#endif
