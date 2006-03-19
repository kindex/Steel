//---------------------------------------------------------------------------

#ifndef __linux
#pragma hdrstop
#endif
#include "time.h"
#include "SDL.h"

#define NORMAL_FPS 140
#define UPDATE_FPS_TIME 1000

/*
Time systemTime, programTime, programStartTime;//, one = 1.0;
Frame frame, lastframe;
//Time speed, speed2;   // x += 1*speed - x menjaetsja na 1 metr v sekundu
Time lastfpstime, updatefpstime;
bool fpsupdated;

Time getSystemTime() 
{ 
	#ifdef __linux
	return SDL_GetTicks();
	#else
	return GetTickCount(); 
	#endif
}
*/

/*
void Timer::clear()
{
    last = programTime;
}

Time Timer::get()
{
    return programTime - last;
}

Timer timer;
*/

double curFPS;
steelTimeM_t fpsTime, updatefpsTime;
steelFrame_t frame;

void initTime()
{
	SDL_Init(SDL_INIT_TIMER);

	fpsTime		= SDL_GetTicks();
    frame		= 0;
    updatefpsTime = UPDATE_FPS_TIME;
	curFPS		= NORMAL_FPS;
}

bool updateFPS()
{
	steelTimeM_t passedTime=SDL_GetTicks()-fpsTime;
	frame++;
	if ( passedTime>=updatefpsTime )
	{
		curFPS=1000.0*frame/passedTime;
		frame=0;
		fpsTime+=passedTime;
		return true;
	}	
	else
	{
		return false;
	}
}
