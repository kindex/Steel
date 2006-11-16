/*id*********************************************************
	File: audio/audio_engine.h
	Unit: OpenAL Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Звуковой движок.
 ************************************************************/


#ifndef __OPENAL_ENGINE_H
#define __OPENAL_ENGINE_H

#include "../steel.h"

#ifdef LIB_OPENAL

#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <map>

#include "openal/al.h"
#include "openal/alc.h"
//#include "openal/alu.h"
//#include "openal/alut.h"
#include "openal/eax.h"

#include "audio_engine.h"

#include "openal/openal_types.h"

//#include "../objects/audio_sound.h"

//#pragma comment(lib, "openal32.lib")
//#pragma comment(lib, "alut.lib")
#pragma comment(lib, "eaxguid.lib")

using namespace std;

class OpenALEngine: public AudioEngine
{
protected:
//#define A(object) ((AudioObject*)(object))
//#define AS(object) ((AudioSound*)(object))


	void setListenerEnvironment(unsigned long environment);		// extracted from protected ???
	ALboolean CheckALCError();
	ALboolean CheckALError();
	void updateListener(Listener &listener);

public:
	bool soundPlay(Sound* sound); 
	bool soundUpdate(Sound* sound);

public:
	

	void setListener(const Listener &aListener);
	bool init(Config* _conf);
	bool deinit(void);
	bool inject(/*AudioObject *object*/);
	bool process(void);

	Storage* getStorageClass(Object *object) { return new Storage(this); }
	void makeStorageForChildren(Object *object) {};
};

#endif
#endif
