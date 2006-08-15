/*id*********************************************************
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.07.30 00:03:09] Kuzmi4: File Creation.
    Description:
		Звуковой движок.
 ************************************************************/

#ifndef __AUDIO_ENGINE_H
#define __AUDIO_ENGINE_H

#include "../engine.h"
#include "../res/audio/audio.h"

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


class Source
{
public:
	//Source();
	//float getPosition(short i) { return position[i]; }
	//float getVelocity(short i) { return velocity[i]; }
	v3 getPosition() { return position; }
	float *getVelocity() { return velocity; }
	float getGain() const { return gain; }
	float getPitch() const { return pitch; }
	bool isLooped() const { return isLoop; }
	void setPosition(float x, float y, float z) { position[0] = x; position[1] = y; position[2] = z; }
	void setVelocity(float x, float y, float z) { velocity[0] = x; velocity[1] = y; velocity[2] = z; }
	void setGain(float newGain) { gain = newGain; }
	void setPitch(float newPitch) { pitch = newPitch; }
	void setLooped(bool newLoop) { isLoop = newLoop; }
private:
	float velocity[3];
	v3 position;
	bool isLoop;
	float gain;
	float pitch;
};


class AudioEngine : public Engine
{
public:
	Listener listener;

//	virtual bool inject(AudioInterface *object);

	virtual bool process() = 0;

	bool clear();
};


#endif