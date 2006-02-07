//---------------------------------------------------------------------------

#ifndef timeH
#define timeH

#include <windows.h>

typedef double time;
typedef long Frame;


extern Frame frame;
extern bool fpsupdated;



time getSystemTime();


void initTime();
void updateTime();
time getfps();

extern time systemTime, programTime, programStartTime;
extern time speed, speed2;

struct Timer
{
    time last;
    void clear();
    time get();
};
extern Timer timer;

//---------------------------------------------------------------------------
#endif
