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

#ifndef __SOUND_H
#define __SOUND_H

class Sound
{
public:
	uid id;

	Audio* sound;

	v3 position;

	float gain;
	float pitch;
	bool isLoop;
	float rolloffFactor;
	float sourceRelative;
};

#endif