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
	bool supportsInterface(IN OUT Engine& engine, IN const InterfaceId id);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id) { return false; }
	void bindEngine(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
    Config* getConfig() const;
	bool audioPlay(const std::string& sndId);


//private:
	AudioDict audio;
	
};

#endif