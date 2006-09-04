/*id*********************************************************
	File: common/timer.h
	Unit: Timer
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Секундомер. Засекает время, считает промежутки времени.
		Высчитывает FPS.
 ************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "../steel.h"

#include "../common/types.h"
#include <string>

class Timer
{
	bool active;
    double startTime, skip, pausedTime, lastIntervalTime, curIntervalStartTime, fps;
	int frameCnt, lastIntervalFrameCnt, curIntervalFrameCnt, totalFrames;

public:
// Время в секундах от момента clear, исключая промежутки между pause и resume
	steel::time total(void) 
	{ 
		return (steel::time)((timestamp() - startTime) - skip);
	}

// Абсолютное время
	virtual double timestamp(void);
// Обнулить таймер
	virtual void start(void);
	virtual void pause(void)
	{
		active		= false;
		pausedTime = timestamp();
	}
	virtual void resume(void)
	{
		if(!active)
		{
			active	= true;
			skip	+= (timestamp() - pausedTime);
		}
	}
// Сообщение таймеру о том, что кадр был обработан и следует обновить FPS
	virtual void incframe(void);
// Вернуть текушее количество кадров в секунду (FPS)
	virtual steel::time getfps(void);
	virtual std::string getfps_s(void);
};

extern Timer globalTimer;

#endif
