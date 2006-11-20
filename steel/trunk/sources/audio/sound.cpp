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

#include "sound.h"

Sound::Sound(void)
{
	sound = NULL;
	position = v3(0.0f, 0.0f, 0.0f);
	gain = 1.0f;
	pitch = 1.0f;
	isLoop = false;
	rolloffFactor = 0.0f;
	sourceRelative = 0.0f;
}