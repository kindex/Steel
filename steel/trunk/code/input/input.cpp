/*id*********************************************************
	File: input/input.cpp
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Абстрактный класс обработчика ввода с клавиатуры и мыши
 ************************************************************/

#include "input.h"
#include "../res/res_main.h"

bool Input::isPressed(std::string key)
{
	return keyPressed.find(key) != keyPressed.end() && keyPressed[key];
}

bool Input::init(std::string _conf) 
{ 
	alive = true; 
	mouseCaptured = false; 

	conf = resConfig.add(_conf);
	if(!conf)
	{
		log_msg("error input res", "Input config file not found");
		return false;
	}

	if(conf->geti("mouse.capture", 0))
		captureMouse();

	sensetivity = conf->getd("mouse.sensetivity", 1);

	return true; 
}


#define MOUSE_SENS (0.002)

void Input::getMouseDelta(double &dx, double &dy)
{
	dx = lastdx*sensetivity*MOUSE_SENS; lastdx = 0;
	dy = lastdy*sensetivity*MOUSE_SENS; lastdy = 0;
}

