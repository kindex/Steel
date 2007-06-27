/*id*********************************************************
	File: input/input.cpp
	Unit: input
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Абстрактный класс обработчика ввода с клавиатуры и мыши
 ************************************************************/

#include "input.h"
#include "../res/res_main.h"

using namespace std;

bool Input::isPressed(std::string key)
{
	return keyPressed.find(key) != keyPressed.end() && keyPressed[key];
}

#if STEEL_OS == OS_WIN32
void Input::UseWinAPI()
{
	CaptureMouse = &Input::CaptureMouse_WinAPI;
	FreeMouse = &Input::FreeMouse_WinAPI;
	Process = &Input::Process_WinAPI;

	log_msg("input", "Using InputAPI::WinAPI");
}
#endif

#ifdef LIB_SDL
void Input::UseSDL()
{
	CaptureMouse = &Input::CaptureMouse_SDL;
	FreeMouse = &Input::FreeMouse_SDL;
	Process = &Input::Process_SDL;

	log_msg("input", "Using InputAPI::SDL");
}
#endif


bool Input::init(Config* _conf) 
{ 
	alive = true; 
	mouseCaptured = false; 

	conf = _conf;
	if(!conf)
	{
		log_msg("error input res", "Input config file not found");
		return false;
	}

	string InputAPI = conf->gets("InputAPI");

	if(InputAPI == "WinAPI")
	#if STEEL_OS == OS_WIN32
		UseWinAPI();
	#else
		error("graph opengl sdl opengl_info", "Cannot find InputAPI::WinAPI");
	#endif

	if(InputAPI == "SDL")
	#ifdef LIB_SDL
		UseSDL();
	#else
		error("graph opengl sdl opengl_info", "Cannot find InputAPI::SDL");
	#endif

#if STEEL_OS == OS_WIN32
	if(Process == NULL)	UseWinAPI();
#endif

#ifdef LIB_SDL
	if(Process == NULL)	UseSDL();
#endif

	if(Process == NULL)
	{
		error("graph opengl sdl opengl_info", "Cannot find InputAPI");
		return false;
	}

	sensetivity = conf->getd("mouse.sensetivity", 1);
	lastdx = 0;
	lastdy = 0;

	return true; 
}


#define MOUSE_SENS (0.002)

void Input::getMouseDelta(double &dx, double &dy)
{
	dx = lastdx*sensetivity*MOUSE_SENS; lastdx = 0;
	dy = lastdy*sensetivity*MOUSE_SENS; lastdy = 0;
}

void Input::start()
{
	if(conf->geti("mouse.capture", 0))
    {
		(this->*CaptureMouse)();
    }
}