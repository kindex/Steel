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
#include "audio/simple_sound.h"

AudioObject* audioObjectFactory(const string &_class)
{
	if(_class == "simple") return new SimpleSound;

	error("objects", string("audio class '") + _class + "' not found");
	return NULL;
}


AudioObject::AudioObject(): 
	engine(NULL)
{}

bool AudioObject::soundPlay(Sound* sound)
{
	sounds.push_back(sound);
	if (engine != NULL)
	{
		engine->soundPlay(sound);
	}

	return true;
}

bool AudioObject::soundStop(Sound* sound)
{
	if (engine != NULL)
	{
		engine->soundStop(sound);
	}

	return true;
}

bool AudioObject::soundPause(Sound* sound)
{
	if (engine != NULL)
	{
		engine->soundPause(sound);
	}
	return true;
}

void AudioObject::bindEngine(Engine& _engine)
{
	engine = dynamic_cast<AudioInterface*>(&_engine);
	// TODO: start playing all scheduled sounds in sounds array
}


