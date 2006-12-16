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

#include "simple_sound.h"

bool SimpleSound::InitFromConfig(Config *conf)
{
	if(conf == NULL) return false;
	
	originalSound = new Sound();

	originalSound->position = conf->getv3("position");
	originalSound->gain = conf->getf("gain", 1.0f);
	originalSound->pitch = conf->getf("pitch", 1.0f);
	originalSound->isLoop = conf->geti("isLoop", 0) > 0;
	originalSound->rolloffFactor = conf->getf("rolloffFactor", 0.0f);
	originalSound->sourceRelative = conf->geti("sourceRelative", 0);

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
