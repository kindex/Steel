//---------------------------------------------------------------------------

#pragma hdrstop
#include "time.h"

//#define NORMAL_FPS 140
#define NORMAL_FPS 40

time systemTime, programTime, programStartTime, one = 1.0;
Frame frame, lastframe;
time speed, speed2;   // x += 1*speed - x menjaetsja na 1 metr v sekundu

time lastfpstime, fps, updatefpstime;
bool fpsupdated;

time getSystemTime() 
{ 
	return GetTickCount()*0.001; 
}

void initTime()
{
    systemTime = getSystemTime();
    programStartTime = systemTime;
    programTime = 0;
    frame = 0;
    lastframe = 0;
    lastfpstime = programTime;
    fps = NORMAL_FPS;

    speed = 1.0f/NORMAL_FPS;
    speed2 = speed*speed;   // x += 1*speed - x menjaetsja na 1 metr v sekundu
    updatefpstime = 0.1;
}

void updateTime()
{
    frame++;
    programTime = getSystemTime() - programStartTime;
}

time getfps()
{
    if (programTime-lastfpstime>updatefpstime)
    {
        fps = (frame-lastframe) / (programTime-lastfpstime);
        lastfpstime += updatefpstime;

        if (frame-lastframe<30) updatefpstime *= 2.0;
        if (frame-lastframe>30) updatefpstime /= 2.0;

        lastframe = frame;
        fpsupdated = true;


        if (fps>0)
        {
            time lastspeed = speed;
            speed = one/fps;
            if (speed>2*lastspeed) speed = 1.1f*lastspeed;


            speed2 = speed*speed;
        }
    }
    else
        fpsupdated = false;
    return fps;
}


void Timer::clear()
{
    last = programTime;
}

time Timer::get()
{
    return programTime - last;
}

Timer timer;

