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

#include "audio_engine.h"

//#pragma comment(lib, "openal32.lib")
//#pragma comment(lib, "alut.lib")
#pragma comment(lib, "eaxguid.lib")

using namespace std;

class AL_Source : public Source
{
public:
	AL_Source(Audio * sound);
	Audio * sound;
//private:
	ALuint buffer;
	ALuint source;
};

/*
class AL_Listener : public Listener
{
};
*/

//extern void updateListener(Listener);				//??????????????????????????????????????????????????

extern bool initializeOpenAL();
extern void destroyOpenAL();
extern void setListenerEnvironment(unsigned long environment);

extern ALboolean CheckALCError();
extern ALboolean CheckALError();

extern void updateListener(Listener &listener);

extern void soundPlay(AL_Source &sound);
extern void soundClose(AL_Source &sound);
extern void soundStop(AL_Source &sound);
extern void soundUpdate(AL_Source &sound);



#endif