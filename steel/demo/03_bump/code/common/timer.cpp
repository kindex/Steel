//---------------------------------------------------------------------------

#ifndef __linux
#pragma hdrstop
#endif
#include "timer.h"
#include <SDL.h>
#include "../utils.h"

#define NORMAL_FPS		(100.0)
#define UPDATE_FPS_TIME (1.0)

void Timer::incframe()
{
	frameCnt++;
	totalFrames++;
}


time Timer::getfps()
{
	time ts = total();
	time cur = ts - curIntervalStartTime;
	if(cur >= UPDATE_FPS_TIME)
	{
		lastIntervalTime = cur;
		curIntervalStartTime = ts;
		lastIntervalFrameCnt = frameCnt;
		frameCnt = 0;
	}
	if(lastIntervalTime > 0.0)
	{
		fps = lastIntervalFrameCnt/lastIntervalTime;
	}
	return fps;
}

std::string Timer::getfps_s()
{
	return FloatToStr(getfps());
}



time Timer_SDL::timestamp()
{
	return SDL_GetTicks()*0.001;
}
