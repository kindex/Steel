/*id*********************************************************
	File: objects/combiner/audio/simple_sound.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Простой аудио-объект, который имеет всего один звук.
 ************************************************************/

#ifndef __SIMPLE_SOUND_H
#define __SIMPLE_SOUND_H

#include "../audio_object.h"

class SimpleSound: public AudioObject
{
public:
	bool isSuportingInterface(IN OUT Engine& engine);
	bool beforeInject(IN OUT Engine&){ return true; }
	void afterRemove(IN OUT Engine&){}
	bool updateInformation(IN OUT Engine&){return false;}
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);

	//v3 getPosition() { return position; }
	//void setPosition(v3 newPosition) { position = newPosition; }

	Sound*	originalSound;		// must be vector		// ???
private:
	
	float	delay;
	bool	started;

	//v3 position;
};

#endif
