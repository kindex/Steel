//---------------------------------------------------------------------------

#ifndef timeH
#define timeH

#include "../_cpp.h"

#include "../common/types.h"
#include <string>

class Timer
{
	bool active;
    steel::time startTime, skip, pausedTime, lastIntervalTime, curIntervalStartTime, fps;
	int frameCnt, lastIntervalFrameCnt, curIntervalFrameCnt, totalFrames;

public:
// ����� � �������� �� ������� clear, �������� ���������� ����� pause � resume
	steel::time total() 
	{ 
		return (timestamp() - startTime) - skip; 
	}

// ���������� �����
	virtual steel::time timestamp(); 
// �������� ������
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
// ��������� ������� � ���, ��� ���� ��� ��������� � ������� �������� FPS
	virtual void incframe();
// ������� ������� ���������� ������ � ������� (FPS)
	virtual steel::time getfps();
	virtual std::string getfps_s();

};


//---------------------------------------------------------------------------
#endif
