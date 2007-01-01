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
	steel::time total() 
	{ 
		return (steel::time)((timestamp() - startTime) - skip);
	}

// Абсолютное время
	virtual double timestamp();
// Обнулить таймер
	virtual void start();
	virtual void pause()
	{
		active		= false;
		pausedTime = timestamp();
	}
	virtual void resume()
	{
		if(!active)
		{
			active	= true;
			skip	+= (timestamp() - pausedTime);
		}
	}
// Сообщение таймеру о том, что кадр был обработан и следует обновить FPS
	virtual void incframe();
// Вернуть текушее количество кадров в секунду (FPS)
	virtual steel::time getfps();
	virtual std::string getfps_s();
};

extern Timer globalTimer;

#endif
