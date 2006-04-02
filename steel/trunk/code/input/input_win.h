#ifndef INPUT_WIN_H
#define INPUT_WIN_H

#include "../_cpp.h"

#ifdef OPENGL_WIN

#include "input.h"
#include <windows.h>

class InputWIN: public Input
{
protected:
	int cx, cy, lastdx, lastdy;

public:
	void captureMouse();
	void freeMouse();
	void process();
	LRESULT CALLBACK processMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void getMouseDelta(double &dx, double &dy);
	void setMouseCenter(int _cx, int _cy) { cx = _cx; cy = _cy; }
};

#endif // OPENGL_WIN
#endif // INPUT_WIN_H


