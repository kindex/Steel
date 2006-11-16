/*id*********************************************************
	Unit: Audio Object
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Звуковой движок. Физический аудио-объект. Interface.
 ************************************************************/

#ifndef __AUDIO_OBJECT_H
#define __AUDIO_OBJECT_H

#include "../steel.h"
#include "../engine/object.h"
#include "../res/audio/audio.h"
#include "sound.h"
#include "audio_engine.h"

class AudioObject: public Object
{
public:
	bool soundPlay(Sound* sound);
	bool soundStop(Sound* sound);

	bool attachAudioEngine(AudioEngine* aEngine);

private:
	AudioEngine* engine;
	vector<Sound*> sounds;	// ???
};

#endif
