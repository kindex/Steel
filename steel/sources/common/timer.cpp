/*id*********************************************************
	File: common/timer.cpp
	Unit: Timer
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Секундомер. Засекает время, считает промежутки времени.
		Высчитывает FPS.
 ************************************************************/
#include "../steel.h"
#include "timer.h"

#include <sys/timeb.h>
#include <time.h>


#include "../common/utils.h"

#define NORMAL_FPS		(100.0)
#define UPDATE_FPS_TIME (0.25)

void Timer::incframe()
{
	frameCnt++;
	totalFrames++;
	lastlap = current();
    updatetotal();
}

steel::time Timer::getfps()
{
	double ts = total();
	double cur = ts - curIntervalStartTime;
	if (cur >= UPDATE_FPS_TIME)
	{
		lastIntervalTime = cur;
		curIntervalStartTime = ts;
		lastIntervalFrameCnt = frameCnt;
		frameCnt = 0;
	}
	if (lastIntervalTime > 0.0)
	{
		fps = lastIntervalFrameCnt/lastIntervalTime;
	}
	return (steel::time)fps;
}

std::string Timer::getfps_s()
{
	return FloatToStr(getfps());
}

double Timer::timestamp() const
{
	#if STEEL_OS == OS_WIN32
	struct _timeb a;
	_ftime(&a);
	 
	#elif STEEL_OS == OS_LINUX
	struct timeb a;
	ftime(&a);
	#endif	// STEEL_OS

	return a.millitm*0.001 + a.time;
}

void Timer::start()
{
	startTime	= timestamp();
	skip		= 0.0;
	active		= true;
	frameCnt	= 0;
	fps			= -1.0;
	lastlap		= 0;
	lastIntervalTime		= 0.0;
	curIntervalStartTime	= 0;
	lastIntervalFrameCnt	= 0;
	curIntervalFrameCnt		= 0;
	totalFrames				= 0;
    currentTime             = 0;
}

steel::time Timer::total() const
{
	if (active)
	{
		return (steel::time)((timestamp() - startTime) - skip);
	}
	else
	{
		return (steel::time)((pausedTime - startTime) - skip);
	}
}

void Timer::updatetotal()
{
    currentTime = total();
}

void Timer::pause()
{
	active		= false;
	pausedTime  = timestamp();
}

void Timer::resume()
{
	if(!active)
	{
		active	= true;
		skip	+= (timestamp() - pausedTime);
	}
}

steel::time Timer::current() const
{
    return currentTime;
}

steel::time Timer::lap() const
{
	return (steel::time)(total() - lastlap);
}

steel::time Timer::last() const
{
	return (steel::time)(lastlap);
}

steel::time Timer::over(steel::time timestamp) const
{
    steel::time t = current();
    steel::time l = last();
	return l < timestamp && timestamp <= t;
}

void Timer::add(steel::time addedTime)
{
	skip -= addedTime;
}

Timer globalTimer;

