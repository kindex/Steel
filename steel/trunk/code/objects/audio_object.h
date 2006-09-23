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

#include "../audio/audio_interface.h"

class AudioSourceRes: public AudioObject
{
public:
	AudioSourceRes(void); //:sound(NULL), gain(1.0f), pitch(1.0f), isLooped(false) {}

	void setSound(Audio *aSound) { sound = aSound; }
	void setGain(float newGain) { gain = newGain; }
	void setPitch(float newPitch) { pitch = newPitch; }
	void setLooped(bool newLoop) { isLoop = newLoop; }

	float getGain(void) { return gain; }
	float getPitch(void) { return pitch;}
	bool isLooped(void) { return isLoop;}
	Audio *getSound(void) {return sound; } 

	

protected:
	Audio *sound;

private:
	float gain;
	float pitch;
	bool isLoop;
};
