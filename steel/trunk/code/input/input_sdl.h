#ifndef INPUT_SDL_H
#define INPUT_SDL_H

#include "../_cpp.h"

#ifdef OPENGL_SDL

#include "input.h"

class InputSDL: public Input
{
protected:
	int sx, sy, cx, cy, mx, my, lastdx, lastdy;
	bool firstMouse;
public:
	void process();
	void captureMouse(int _cx, int _cy);
	void getMouseDelta(int &dx, int &dy);

	bool init(std::string _conf, Game *_game) 
	{ 
		if(!Input::init(_conf, _game)) return false;
		firstMouse = true;
		lastdx = 0;
		lastdy = 0;
		return true; 
	}
};
#endif // OPENGL_SDL
#endif // INPUT_SDL_H
