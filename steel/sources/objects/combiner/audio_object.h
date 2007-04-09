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
#include <map>

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
	
	void setPosition(v3 newPosition) { position = newPosition; }
//protected:
	bool soundPlay(Sound*);
	bool soundStop(Sound*);
	bool soundPause(Sound*);
	bool soundUpdate(Sound*);
	AudioInterface* engine;

	std::string sndId;
	v3 position;
protected:
	
	
	//std::vector<Sound*> sounds;	// ???						// ???
//	AudioMap audio;
	//typedef std::map<uid, AudioShadow*> AudioShadowMap;
};

#endif
