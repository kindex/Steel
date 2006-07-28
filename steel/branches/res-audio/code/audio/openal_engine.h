#ifndef __OPENAL_ENGINE_H
#define __OPENAL_ENGINE_H

#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <map>

#include "openal/al.h"
#include "openal/alc.h"
#include "openal/alu.h"
#include "openal/alut.h"
#include "openal/eax.h"

//#pragma comment(lib, "openal32.lib")
//#pragma comment(lib, "alut.lib")
#pragma comment(lib, "eaxguid.lib")

using namespace std;

typedef struct
{
    unsigned int ID;
	string fileName;
    unsigned int Rate;
    unsigned int Format;
} SndInfo;

typedef map<ALuint, SndInfo> TBuf;

extern TBuf buffers;

extern bool initializeOpenAL();
extern void destroyOpenAL();
extern void setListenerEnvironment(unsigned long environment);

extern ALboolean CheckALCError();
extern ALboolean CheckALError();


#endif