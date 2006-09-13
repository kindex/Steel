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
protected:
	Audio *sound;
public:
	AudioSourceRes(void):sound(NULL) {}

	void setSound(Audio *aSound) { sound = aSound; }

	float getGain(void) { return 1.0f;}
	float getPitch(void){ return 1.0f;}
	bool isLooped(void) { return false;}
	Audio *getSound(void) {return sound; } 
};
