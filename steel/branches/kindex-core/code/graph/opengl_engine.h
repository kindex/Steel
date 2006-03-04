#ifndef __GRAPH_OPENGL_ENGINE_H
#define __GRAPH_OPENGL_ENGINE_H

#include "graph_engine.h"
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include <string>

struct Window
{
    std::string caption, title; // if not fullscreen

    HWND handle;
    HDC DC;    // Window Handle, Device Context
    HGLRC RC; // Rendering Context - for OpenGL
    DWORD dwStyle;

    int width, height, left, top, bpp;
	bool needupdate; // true on resize

 //   HINSTANCE hInstance;
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

