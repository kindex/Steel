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
#include "sound.h"
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


class AudioEngine : public Engine, public AudioInterface
{
public:
	AudioEngine() :  masterVolume(1.0f) {}

	virtual bool init(Config& _conf) abstract;
	virtual bool clear() { return true; } 
	virtual void pause()  abstract; 
	virtual void unpause()  abstract; 
	virtual bool deinit() abstract;
	virtual void setListener(const Listener &aListener) { listener = aListener; }
	virtual void setListenerEnvironment(unsigned long environment) abstract;

	virtual bool process() abstract;
	virtual bool inject(GameObject& object) abstract;

	float getMasterVolume() { return masterVolume; }

	
	//============================

	Listener listener;
//	std::vector<AudioConfig> config;		// audio configs

protected:
	float masterVolume;
	bool enabled;

	//AudioObjectList sources;


	// the same as Kindex

	//steel::vector<AudioObject*> objects;

};


#endif
