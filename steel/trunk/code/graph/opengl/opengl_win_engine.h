/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine WIN API
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Часть графического движока OpenGL, которая реализует
		системно зависимые функции, такие как создание окна.
 ************************************************************/

// Undone - dirty

#ifndef __GRAPH_OPENGL_WIN_ENGINE_H
#define __GRAPH_OPENGL_WIN_ENGINE_H

#include "../../_cpp.h"
#include "../../steel.h"

#if STEEL_VIDEOSYS == OPENGL_WIN

#include "opengl_engine.h"
#include "../../common/logger.h"
#include "../../input/input_win.h"


class OpenGL_WIN_Engine: public OpenGL_Engine
{
protected:
	void swapBuffers();
	bool createWindow();

	HWND	handle;
	HDC		DC;    // Window Handle, Device Context
	HGLRC	RC; // Rendering Context - for OpenGL
	DWORD	dwStyle;
	InputWIN *input;
public:
	bool return_pressed;

public:
	bool init(std::string _conf, InputWIN *_input);

	OpenGL_WIN_Engine() {}
	bool deinit();
	void setCaption(std::string caption);
	void resize(int w, int h);
	void repair(); // repair window on resize
	void fullScpeenToggle();
	bool changeToFullScpeen();
	bool changeToWindow();
};

#endif	// STEEL_VIDEOSYS
#endif
