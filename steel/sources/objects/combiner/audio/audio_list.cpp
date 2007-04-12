/*id*********************************************************
	File: objects/combiner/audio/audio_list.cpp
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Сложный аудио-объект, который имеет много звуков (состоит из многих простых аудио-объектов).
 ************************************************************/

#include "audio_list.h"
#include "../../../engine/engine_types.h"
#include "../../../res/config/config.h"
#include "../../../res/res_main.h"
#include "../../../audio/sound.h"


bool AudioList::InitFromConfig(IN Config& conf)
{
	//AudioObject::InitFromConfig(conf);

	ConfigArray* list = conf.getArray("sounds");
	if (list != NULL)
	{
		for EACH(ConfigArray, *list, it)
		{
			AudioObject* newAudioObject = audioObjectFactory((*it)->gets("class"));
			if (newAudioObject != NULL)
			{
				if (newAudioObject->InitFromConfig(**it))
				{
					//transformations.push_back(newTransformation);
					audio[newAudioObject->sndId] = (SimpleSound*) newAudioObject;
				}
				else
				{
					delete newAudioObject;
				}
			}
		}
	}

	return true;
}


void AudioList::bindEngine(Engine& aEngine)
{
	for EACH(AudioDict, audio, it)
		it->second->bindEngine(aEngine);
}

void AudioList::process(IN const ProcessInfo& info)
{
	for EACH(AudioDict, audio, it)
	{
		it->second->position = position;
		it->second->process(info);
	}
}

bool AudioList::isSuportingInterface(IN OUT Engine& engine)
{
	return engine.isSupportingInterface(INTERFACE_AUDIO);
}

bool AudioList::audioPlay(const std::string& sndId)
{
	AudioDict::iterator it = audio.find(sndId);
	if (it != audio.end())
	{
		soundPlay(it->second->originalSound/*audio[sndId]->originalSound*/);
		return true;
	}
	return false;
}

