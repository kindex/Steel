#ifndef INPUT_WIN_H
#define INPUT_WIN_H

#include "../_cpp.h"

#ifdef OPENGL_WIN

#include "input.h"

class InputWIN: public Input
{
protected:
	int cx, cy, lastdx, lastdy;

public:
	void captureMouse(int _cx, int _cy);
	void process();
	void getMouseDelta(int &dx, int &dy);
};

#endif // OPENGL_WIN
#endif // INPUT_WIN_H


