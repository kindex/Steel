/*id*********************************************************
	File: input/input_sdl.h
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Обработчик ввода с клавиатуры и мыши, используя SDL
 ************************************************************/

#ifndef INPUT_SDL_H
#define INPUT_SDL_H

#include "../steel.h"

#if STEEL_OPENGL_API == OPENGL_SDL

#include "input.h"

class InputSDL: public Input
{
protected:
	//int sx, sy, mx, my;
	//bool firstMouse;
public:
	void process(void);
	void captureMouse(void);
	void freeMouse(void);

	bool init(std::string _conf)
	{ 
		if(!Input::init(_conf)) return false;
		//firstMouse = true;
		lastdx = 0;
		lastdy = 0;
		return true; 
	}
};
#endif // STEEL_OPENGL_SDL
#endif // INPUT_SDL_H
