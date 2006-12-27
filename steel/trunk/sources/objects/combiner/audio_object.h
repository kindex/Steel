/*id*********************************************************
	File: objects/combiner/audio_object.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Физический аудио-объект.
 ************************************************************/

#ifndef __AUDIO_OBJECT_H
#define __AUDIO_OBJECT_H

#include "../../steel.h"
#include "../../engine/game_object.h"
#include "../../res/audio/audio.h"
#include "../../audio/sound.h"
#include "../../audio/audio_engine.h"

class AudioObject;

AudioObject* findAudioObject(const string &_class);

class AudioObject: public GameObject
{
public:
	AudioObject(void);
	bool isSuportingInterface(InterfaceId id) { return AudioInterface::interfaceId == id; }
	void bindEngine(InterfaceId, Engine*);
	
//protected:
	AudioInterface* engine;
	bool soundPlay(Sound* sound);
	bool soundStop(Sound* sound);
	bool soundPause(Sound* sound);

private:
	
	vector<Sound*> sounds;	// ???						// ???
};

#endif
