/*id*********************************************************
	File: input/input_win.h
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Обработчика ввода с клавиатуры и мыши, используя Windows API
 ************************************************************/

#ifndef INPUT_WIN_H
#define INPUT_WIN_H

#include "../steel.h"

#if STEEL_OPENGL_API == OPENGL_WINAPI

#include "input.h"
#include <windows.h>

std::string decodeKey(WPARAM p);


class InputWIN: public Input
{
protected:

public:
	void captureMouse(void);
	void freeMouse(void);
	void process(void);
	LRESULT CALLBACK processMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // OPENGL_WINAPI
#endif // INPUT_WIN_H
