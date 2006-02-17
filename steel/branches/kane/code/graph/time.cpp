//---------------------------------------------------------------------------

#ifndef __linux
#pragma hdrstop
#endif
#include "time.h"
#ifdef __linux
#include "SDL.h"
#endif

//#define NORMAL_FPS 140
#define NORMAL_FPS 40

Time systemTime, programTime, programStartTime;//, one = 1.0;
Frame frame, lastframe;
//Time speed, speed2;   // x += 1*speed - x menjaetsja na 1 metr v sekundu

Time lastfpstime, updatefpstime;
bool fpsupdated;

Time getSystemTime() 
{ 
	#ifdef __linux
	return SDL_GetTicks();
	#else
	return GetTickCount(); 
	#endif
}

void initTime()
{
    systemTime = getSystemTime();
    programStartTime = systemTime;
    programTime = 0;
    frame = 0;
    //lastframe = 0;
    lastfpstime = programTime;
    //fps = NORMAL_FPS;

  //  speed = 1.0f/NORMAL_FPS;
  //  speed2 = speed*speed;   // x += 1*speed - x menjaetsja na 1 metr v sekundu
    updatefpstime = 1000;
}

void updateTime()
{
    frame++;
    programTime = getSystemTime() - programStartTime;
}

double getfps()
{
	double fps;
    if (programTime-lastfpstime>updatefpstime)
    {
        fps = 1000.0 * (double)frame / (programTime-lastfpstime);
        //lastfpstime += updatefpstime;
		lastfpstime=programTime;

        //if (frame-lastframe<30) updatefpstime *= 2.0;
        //if (frame-lastframe>30) updatefpstime /= 2.0;

        //lastframe = frame;
		frame=0;
        fpsupdated = true;


        /*
		if (fps>0)
        {
            time lastspeed = speed;
            speed = one/fps;
            if (speed>2*lastspeed) speed = 1.1f*lastspeed;


            speed2 = speed*speed;
        }
		*/
    }
    else
        fpsupdated = false;
    return fps;
}


void Timer::clear()
{
    last = programTime;
}

Time Timer::get()
{
    return programTime - last;
}

Timer timer;
