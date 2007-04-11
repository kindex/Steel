/*id*********************************************************
	File: objects/combiner/audio/audio_list.h
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

#ifndef __AUDIO_LIST
#define __AUDIO_LIST

#include "../audio_object.h"
#include "simple_sound.h"

typedef std::map<std::string, SimpleSound*> AudioDict;

class AudioList: public AudioObject
{
public:
	bool isSuportingInterface(IN OUT Engine& engine);
	bool beforeInject(IN OUT Engine&){ return true; }
	void afterRemove(IN OUT Engine&){}
	bool updateInformation(IN OUT Engine&){return false;}
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
	bool audioPlay(const std::string& sndId);


//private:
	AudioDict audio;
	
};

#endif