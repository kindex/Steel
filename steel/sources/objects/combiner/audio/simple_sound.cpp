/*id*********************************************************
	File: objects/combiner/audio/simple_sound.cpp
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
#include "../../../res/res_main.h"
#include "../../../audio/sound.h"

bool SimpleSound::InitFromConfig(Config& conf)
{
	originalSound = new Sound();

	originalSound->position = conf.getv3("position");
	//float master = engine->getMasterVolume(); 
	originalSound->gain = conf.getf("gain", 1.0f); //* master;
	originalSound->pitch = conf.getf("pitch", 1.0f);
	originalSound->isLoop = conf.geti("isLoop", 0) > 0;
	originalSound->rolloffFactor = conf.getf("rolloffFactor", 0.0f);
	originalSound->sourceRelative = conf.geti("sourceRelative", 0);

	originalSound->sound = resAudio.add(conf.getPath("file"));
	delay = conf.getf("delay", 0.0);
	started = false;
	//engine->soundPlay(originalSound);

	return true;
}

void SimpleSound::bindEngine(Engine& aEngine)
{
	if (aEngine.isSupportingInterface(INTERFACE_AUDIO))
	{		
		AudioObject::bindEngine(aEngine);
		if (delay <= 0.0 && !started)
		{
			//soundPlay(originalSound);
			//started = true;
		}
	}
}

void SimpleSound::process(IN const ProcessInfo& info)
{
	originalSound->position = position;
	if (!started && info.timeInfo.currentTime > delay)
	{
		soundPlay(originalSound);
		started = true;
	}
	if (started)
	{
		soundUpdate(originalSound);
	}
}

bool SimpleSound::isSuportingInterface(IN OUT Engine& engine)
{
	return engine.isSupportingInterface(INTERFACE_AUDIO);
}
