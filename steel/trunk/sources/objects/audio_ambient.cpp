/*id*********************************************************
	File: objects/audio_ambient.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Аудио объект - окружающий звук.
		Специально для Дефа - это например не звуки ламп в 
		помещении. Это тоже самое как  музыка в Quake.
		Лампы в помещении - это полноценные источники звука.
 ************************************************************/

#include "audio_ambient.h"


AudioAmbient::AudioAmbient(void)
{
	setSound(NULL);
	setGain(0.5f);
	setPitch(1.0f);
	setLooped(true);
}