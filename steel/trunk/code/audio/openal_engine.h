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
/*
class AL_Source : public Source
{
public:
	//AL_Source(Audio * sound);

	Audio *sound;
//private:
	ALuint buffer;
	ALuint source;

	std::string name;	// == id
};

/*
class AL_Listener : public Listener
{
};
*/

//extern void updateListener(Listener);				

class OpenALEngine: public AudioEngine
{
protected:
#define A(object) ((AudioObject*)(object))
	struct AudioStorage: public Storage // множество треугольников одного материала
	{
		Audio *sound;
		v3 position;

		// from OpenAL_Source
		unsigned int source;		// unsigned int
		unsigned int buffer;		// unsigned int
		bool isLoop;
		float gain;
		float pitch;

		void soundPlay(void);
		void soundClose(void);
		void soundStop(void);
		void soundUpdate(void);

		bool init(void);
		bool deinit(void) { return true; }
		void fill(Object *object);
		bool cache(void);

		AudioStorage(Engine *engine): Storage(engine), sound(NULL) {}
		~AudioStorage(void) { deinit(); }
	};


	void setListenerEnvironment(unsigned long environment);
	ALboolean CheckALCError();
	ALboolean CheckALError();
	void updateListener(Listener &listener);

public:
	void setListener(const Listener &aListener);
	bool init(const std::string _conf);
	bool deinit(void);
	bool inject(AudioObject *object);

	Storage* getStorageClass(Object *object) { return new AudioStorage(this); }
	void makeStorageForChildren(Object *object);
};

#endif

