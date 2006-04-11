#include "input.h"

bool Input::isPressed(std::string key)
{
	return keyPressed.find(key) != keyPressed.end() && keyPressed[key];
}

bool Input::init(ResCollection *_res, std::string _conf) 
{ 
	res = _res;
	alive = true; 
	mouseCaptured = false; 

	conf = (Config*)res->add(Res::config, _conf);
	if(!conf)
	{
		alog.msg("error input res", "Input config file not found");
		return false;
	}

	if(conf->geti("mouse.capture", 0))
		captureMouse();

	sensetivity = conf->getd("mouse.sensetivity", 1);

	return true; 
}


#define MOUSE_SENS (0.02)

void Input::getMouseDelta(double &dx, double &dy)
{
	dx = lastdx*sensetivity*MOUSE_SENS; lastdx = 0;
	dy = lastdy*sensetivity*MOUSE_SENS; lastdy = 0;
}

