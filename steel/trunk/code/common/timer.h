//---------------------------------------------------------------------------

#ifndef timeH
#define timeH

#include "../common/types.h"
#include <string>

class Timer
{
	bool active;
    steel::time startTime, skip, pausedTime, lastIntervalTime, curIntervalStartTime, fps;
	int frameCnt, lastIntervalFrameCnt, curIntervalFrameCnt, totalFrames;

public:
// Время в секундах от момента clear, исключая промежутки между pause и resume
	steel::time total() 
	{ 
		return (timestamp() - startTime) - skip; 
	}

// Абсолютное время
	virtual steel::time timestamp() = 0; 
// Обнулить таймер
	virtual void start()
	{
//		SDL_Init(SDL_INIT_TIMER);

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

class Timer_SDL: public Timer
{
public:
	steel::time timestamp();

};

//---------------------------------------------------------------------------
#endif
