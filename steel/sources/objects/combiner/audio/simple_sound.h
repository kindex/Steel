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
	bool supportsInterface(IN OUT Engine& engine, IN const InterfaceId id);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id){return false;}
	void bindEngine(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
    Config* getConfig() const;

	//v3 getPosition() { return position; }
	//void setPosition(v3 newPosition) { position = newPosition; }

	Sound*	originalSound;		// must be vector		// ???
private:
	
	float	delay;
	bool	started;

	//v3 position;
};

#endif
