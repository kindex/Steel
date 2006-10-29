/*id*********************************************************
	File: audio/audio_engine.h
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Звуковой движок.
 ************************************************************/

#ifndef __AUDIO_ENGINE_H
#define __AUDIO_ENGINE_H

#include "../steel.h"
#include <vector>
#include "../engine/engine.h"
#include "../res/audio/audio.h"
#include "../res/res_main.h"
#include "audio_interface.h"

class AudioEngine;

class Listener
{
public:
	Listener();
	~Listener() {};
	void setPosition(float x, float y, float z) { position[0] = x; position[1] = y; position[2] = z; }
	void setVelocity(float x, float y, float z) { velocity[0] = x; velocity[1] = y; velocity[2] = z; }
	void setOrientation(v3 at, v3 up) { atVect = at; upVect = up; }
	v3 getPosition() { return position; }
	v3 getVelocity() { return velocity; }
	//float *getOrientation() { return orientation; }
	v3 getOrientationAt() { return atVect; }
	v3 getOrientationUp() { return upVect; }
private:
	v3 position;
	v3 velocity;
	//float orientation[6];
	v3 atVect;
	v3 upVect;
};


class AudioEngine : public Engine
{
protected:
	svector<AudioObject*> objects;

public:
	AudioEngine(void) {}

	virtual bool init(Config *_conf) = 0;
	virtual bool clear(void) { return true; } 
	virtual bool deinit(void) = 0;
	virtual void setListener(const Listener &aListener) { listener = aListener; }
	virtual void setListenerEnvironment(unsigned long environment) = 0;

	virtual bool inject(AudioObject *object) = 0;

//============================

	Listener listener;
//	std::vector<AudioConfig> config;		// audio configs


	//AudioObjectList sources;


	// the same as Kindex

	//steel::vector<AudioObject*> objects;

};


#endif
