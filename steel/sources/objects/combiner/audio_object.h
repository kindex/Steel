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
class Sound;
class AudioInterface;
class AudioObject;

AudioObject* audioObjectFactory(const std::string& _class);

class AudioObject: public GameObject
{
public:
	AudioObject();
	bool isSuportingInterface(Engine& engine);
	void bindEngine(Engine&);
	
//protected:
	bool soundPlay(Sound* sound);
	bool soundStop(Sound* sound);
	bool soundPause(Sound* sound);
	AudioInterface* engine;

private:
	std::vector<Sound*> sounds;	// ???						// ???
};

#endif
