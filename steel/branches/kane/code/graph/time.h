//---------------------------------------------------------------------------

#ifndef timeH
#define timeH

#ifndef __linux
#include <windows.h>
#endif

typedef unsigned int Time;
typedef long Frame;


extern Frame frame;
extern bool fpsupdated;



Time getSystemTime();


void initTime();
void updateTime();
double getfps();

extern Time systemTime, programTime, programStartTime;
extern Time speed, speed2;

struct Timer
{
    Time last;
    void clear();
    Time get();
};
extern Timer timer;

//---------------------------------------------------------------------------
#endif
