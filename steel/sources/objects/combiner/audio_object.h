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

AudioObject* audioObjectFactory(const string &_class);

class AudioObject: public GameObject
{
public:
	AudioObject();
	bool isSuportingInterface(Engine& engine)
	{
		return engine.isSupportingInterface(INTERFACE_AUDIO);
	}
	void bindEngine(Engine&);
	
//protected:
	bool soundPlay(Sound* sound);
	bool soundStop(Sound* sound);
	bool soundPause(Sound* sound);
	AudioInterface* engine;

private:
	vector<Sound*> sounds;	// ???						// ???
};

#endif
