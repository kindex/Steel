
#include "timer.h"
#include "../_cpp.h"

#include <sys/timeb.h>
#include <time.h>


#include "../utils.h"

#define NORMAL_FPS		(100.0)
#define UPDATE_FPS_TIME (0.25)

void Timer::incframe()
{
	frameCnt++;
	totalFrames++;
}


steel::time Timer::getfps()
{
	steel::time ts = total();
	steel::time cur = ts - curIntervalStartTime;
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



steel::time Timer_SDL::timestamp()
{
	 struct _timeb a;
	_ftime(&a);

	return a.millitm*0.001 + a.time;
}

void Timer::start()
{
	startTime	= timestamp();
	skip	= 0.0;
	active	= true;
	frameCnt = 0;
	fps		= -1.0;
	lastIntervalTime		= 0.0;
	curIntervalStartTime	= startTime;
	lastIntervalFrameCnt	= 0;
	curIntervalFrameCnt		= 0;
	totalFrames	= 0;
}
