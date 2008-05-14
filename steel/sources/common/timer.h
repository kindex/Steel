/*id*********************************************************
	File: common/timer.h
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

#ifndef __TIMER_H__
#define __TIMER_H__

#include "../steel.h"

#include "../common/types.h"
#include <string>

class Timer
{
public:
// Время в секундах от момента clear, исключая промежутки между pause и resume
	steel::time total() const;
	void updatetotal();
	steel::time lap() const;
	steel::time last() const;
	steel::time current() const;
	steel::time over(steel::time time) const;
	void add(steel::time);

// Абсолютное время
	virtual double timestamp() const;
// Обнулить таймер
	virtual void start();
	virtual void pause();
	virtual void resume();
// Сообщение таймеру о том, что кадр был обработан и следует обновить FPS
	virtual void incframe();
// Вернуть текушее количество кадров в секунду (FPS)
	virtual steel::time getfps();
	virtual std::string getfps_s();

private:
	bool active;
    double startTime;
	double skip;
	double pausedTime;
	double lastIntervalTime;
	double curIntervalStartTime;
	double fps;
	double lastlap;
    steel::time currentTime;
	int frameCnt;
	int lastIntervalFrameCnt;
	int curIntervalFrameCnt;
	int totalFrames;
};

extern Timer globalTimer;

#endif
