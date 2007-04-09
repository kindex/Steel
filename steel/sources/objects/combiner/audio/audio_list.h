#ifndef __AUDIO_LIST
#define __AUDIO_LIST

#include "../audio_object.h"
#include "simple_sound.h"

typedef std::map<std::string, SimpleSound*> AudioMap;

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

private:
	AudioMap audio;
};

#endif