//---------------------------------------------------------------------------

#ifndef timeH
#define timeH

#include "../common/types.h"

/*
extern steelFrame_t frame;
extern bool fpsupdated;

void initTime();
void updateTime();
double getfps();

extern Time systemTime, programTime, programStartTime;
extern Time speed, speed2;

*/

extern double curFPS;
void initTime();
bool updateFPS();

/*
struct Timer
{
    Time last;
    void clear();
    Time get();
};
extern Timer timer;
*/
//---------------------------------------------------------------------------
#endif
