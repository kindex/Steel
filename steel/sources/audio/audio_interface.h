/*id*********************************************************
	File: audio/audio_interface.h
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Интерфейс звукового движок.
 ************************************************************/

#ifndef __AUDIO_INTERFACE_H
#define __AUDIO_INTERFACE_H

#include "../steel.h"

#include "sound.h"

class AudioInterface: public BaseInterface, public ChildrenInterface, public Interface3D
{
public:
	virtual bool soundPlay(Sound* sound) abstract;
	virtual bool soundStop(Sound* sound) abstract;
	virtual bool soundPause(Sound* sound) abstract;
	virtual bool soundUpdate(Sound* sound) abstract;

	virtual float getMasterVolume() abstract;
};



#endif
