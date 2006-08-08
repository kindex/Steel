#ifndef __AUDIO_ENGINE_H
#define __AUDIO_ENGINE_H

#include "../res/audio/audio.h"

class AudioEngine;

class Listener
{
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

/*
class AudioEngine : public Engine
{
public:
	Listener listener;
};
*/

#endif