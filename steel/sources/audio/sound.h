/*id*********************************************************
	Unit: Audio Object
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Звуковой движок. Тип звука. Набор параметров.
 ************************************************************/

#include "../math/vector3d.h"
#include "../engine/id_generator.h"

class Audio;

#ifndef __SOUND_H
#define __SOUND_H

class Sound
{
public:
	Sound();

	Audio* sound;

	uid id;
	v3 position;

	float gain;
	float pitch;
	bool isLoop;
	float rolloffFactor;
	int sourceRelative;
};

#endif
