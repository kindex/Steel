//---------------------------------------------------------------------------

#ifndef timeH
#define timeH

#include "../common/types.h"
#include <string>

class Timer
{
	bool active;
    time startTime, skip, pausedTime, lastIntervalTime, curIntervalStartTime, fps;
	int frameCnt, lastIntervalFrameCnt, curIntervalFrameCnt, totalFrames;

public:
// ����� � �������� �� ������� clear, �������� ���������� ����� pause � resume
	time total() 
	{ 
		return (timestamp() - startTime) - skip; 
	}

// ���������� �����
	virtual time timestamp() = 0; 
// �������� ������
	virtual void start()
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
// ��������� ������� � ���, ��� ���� ��� ��������� � ������� �������� FPS
	virtual void incframe();
// ������� ������� ���������� ������ � ������� (FPS)
	virtual time getfps();
	virtual std::string getfps_s();

};

class Timer_SDL: public Timer
{
public:
	time timestamp();

};

//---------------------------------------------------------------------------
#endif
