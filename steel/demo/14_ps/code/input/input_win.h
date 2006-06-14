#ifndef INPUT_WIN_H
#define INPUT_WIN_H

#include "../_cpp.h"

#ifdef OPENGL_WIN

#include "input.h"
#include <windows.h>

std::string decodeKey(WPARAM p);


class InputWIN: public Input
{
protected:

public:
	void captureMouse();
	void freeMouse();
	void process();
	LRESULT CALLBACK processMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // OPENGL_WIN
#endif // INPUT_WIN_H


