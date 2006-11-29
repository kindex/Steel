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

AudioObject* findAudioObject(const string &_class)
{
	if(_class == "SimpleSound") return new SimpleSound;

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

void AudioObject::bindEngine(InterfaceId id, Engine* aEngine)
{
	if(id == AudioInterface::interfaceId)
	{
		engine = dynamic_cast<AudioInterface*>(aEngine);
	}
}

bool SimpleSound::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	originalSound = new Sound();

	originalSound->position = conf->getv3("position");
	originalSound->gain = conf->getf("gain", 1.0f);
	originalSound->pitch = conf->getf("pitch", 1.0f);
	originalSound->isLoop = conf->geti("isLoop", 0) > 0;
	originalSound->rolloffFactor = conf->getf("rolloffFactor", 0.0f);
	originalSound->sourceRelative = conf->getf("sourceRelative", 0.0f);

	originalSound->sound = resAudio.add(conf->getPath("file"));


	return true;
}

void SimpleSound::bindEngine(InterfaceId id, Engine* aEngine)
{
	if(id == AudioInterface::interfaceId)
	{
		AudioObject::bindEngine(id, aEngine);
		soundPlay(originalSound);
	}
}
