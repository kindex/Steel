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
	for EACH(AudioMap, audio, it)
		it->second->bindEngine(aEngine);
}

void AudioList::process(IN const ProcessInfo& info)
{
	for EACH(AudioMap, audio, it)
	{
		it->second->position = position;
		it->second->process(info);
	}
}

bool AudioList::isSuportingInterface(IN OUT Engine& engine)
{
	return engine.isSupportingInterface(INTERFACE_AUDIO);
}
