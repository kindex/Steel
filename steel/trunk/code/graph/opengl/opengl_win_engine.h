#ifndef __GRAPH_OPENGL_WIN_ENGINE_H
#define __GRAPH_OPENGL_WIN_ENGINE_H

#include "opengl_engine.h"
#include "../../common/logger.h"


class OpenGL_WIN_Engine: public OpenGL_Engine
{
protected:
	void swapBuffers();
	bool createWindow();

	HWND handle;
	HDC DC;    // Window Handle, Device Context
	HGLRC RC; // Rendering Context - for OpenGL
	DWORD dwStyle;

public:
	OpenGL_WIN_Engine() {}
	bool deinit();
	void setCaption(std::string caption);
};

#endif