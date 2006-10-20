/*id*********************************************************
	File: objects/audio_object.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Аудио объект, берущий звуки из ресурсов
 ************************************************************/

#ifndef __AUDIO_OBJECT_H
#define __AUDIO_OBJECT_H

#include "../audio/audio_interface.h"

class AudioSourceRes: public AudioObject
{
public:
	AudioSourceRes(void); //:sound(NULL), gain(1.0f), pitch(1.0f), isLooped(false) {}

	void setSound(Audio *aSound) { sound = aSound; }
	void setGain(float newGain) { gain = newGain; }
	void setPitch(float newPitch) { pitch = newPitch; }
	void setLooped(bool newLoop) { isLoop = newLoop; }

	void setRolloff(float newRolloff) { rolloffFactor = newRolloff; }
	void setSourceRelative(bool newRelation) { sourceRelative = newRelation; }

	float getGain(void) { return gain; }
	float getPitch(void) { return pitch;}
	bool isLooped(void) { return isLoop;}
	Audio* getSound(void) {return sound; } 

	float getRolloff(void) { return rolloffFactor; }
	bool getSourceRelative(void) { return sourceRelative; }
	

protected:
	Audio *sound;

private:
	float gain;
	float pitch;
	bool isLoop;

	float rolloffFactor;	// rolloff factor, If you set AL_ROLLOFF_FACTOR to 0, 
							// you are just disabling distance attenuation, not panning.
	bool sourceRelative;	// source relative to the listener, constant = true / dynamic = false
};

#endif