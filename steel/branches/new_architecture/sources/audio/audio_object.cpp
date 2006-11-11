/*id*********************************************************
	Unit: Audio Object
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		�������� ������. ���������� �����-������. Interface.
 ************************************************************/

#include "audio_object.h"

bool AudioObject::soundPlay(Sound* sound)
{
	sounds.push_back(sound);
	engine->soundPlay(sound);
	return true;
}