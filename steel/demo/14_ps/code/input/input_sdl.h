#ifndef INPUT_SDL_H
#define INPUT_SDL_H

#include "../_cpp.h"

#ifdef OPENGL_SDL

#include "input.h"

class InputSDL: public Input
{
protected:
	int sx, sy, mx, my;
	bool firstMouse;
public:
	void process();
	void captureMouse();
	void freeMouse();

	bool init(ResCollection *_res, std::string _conf)
	{ 
		if(!Input::init(_res, _conf)) return false;
		firstMouse = true;
		lastdx = 0;
		lastdy = 0;
		return true; 
	}
};
#endif // OPENGL_SDL
#endif // INPUT_SDL_H