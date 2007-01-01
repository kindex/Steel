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

	bool soundPlay(Sound* sound);
	bool soundStop(Sound* sound);
	bool soundPause(Sound* sound);
	bool soundUpdate(Sound* sound);

	void setListenerEnvironment(unsigned long environment);		// extracted from protected ???
	ALboolean CheckALCError();
	ALboolean CheckALError();
	void updateListener(Listener &listener);

// AUDIO SHADOW ==============================
	struct AudioShadow: public Shadow
	{
		//Sound* sound;
		Buffer buffer;
		Source source;
		AudioShadow(Engine *aEngine): Shadow(aEngine) {}
	};
// AUDIO SHADOW ==============================

	AudioShadow *currentShadow;
	GameObject *currentObject;

	map<uid, AudioShadow*> shadows;

	// to discuss: vector<Sound*> sounds;	-- "playing" sounds

public:
	

	void setListener(const Listener &aListener);
	bool init(Config* _conf);
	bool deinit();
	bool inject(GameObject *object);
	bool process();


	AudioShadow* getShadowClass(GameObject *object) { return new AudioShadow(this); }

	void addChild(GameObject* child);
	void addChild(AudioShadow &shadow, GameObject *child);

	void deleteChild(GameObject* child){}
	void clearChildren() {}
	void setPosition(ObjectPosition) {}
	void setPositionKind(PositionKind) {}
	bool setCurrentObject(GameObject* object);

	

	// Configuration/Option Parameters
	bool enabledEAX;
	//float masterVolume;
	

//	Storage* getStorageClass(GameObject *object) { return new Storage(this); }
//	void makeStorageForChildren(GameObject *object) {};
};

#endif
#endif
