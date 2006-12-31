/*id*********************************************************
	File: objects/combiner/audio_object.cpp
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


#include "audio_object.h"
#include "simple_sound.h"

AudioObject* findAudioObject(const string &_class)
{
	if(_class == "simple") return new SimpleSound;

	error("objects", string("audio class '") + _class + "' not found");
	return NULL;
}


AudioObject::AudioObject(void): 
	engine(NULL)
{}

bool AudioObject::soundPlay(Sound* sound)
{
	sounds.push_back(sound);
	engine->soundPlay(sound);
	return true;
}

bool AudioObject::soundStop(Sound* sound)
{
	engine->soundStop(sound);
	return true;
}

bool AudioObject::soundPause(Sound* sound)
{
	engine->soundPause(sound);
	return true;
}

void AudioObject::bindEngine(InterfaceId id, Engine* aEngine)
{
	if(id == AudioInterface::interfaceId)
	{
		engine = dynamic_cast<AudioInterface*>(aEngine);
	}
}


