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
		��������� ��������� ������.
 ************************************************************/

#ifndef __AUDIO_INTERFACE_H
#define __AUDIO_INTERFACE_H

#include "../steel.h"

#include "sound.h"

class AudioInterface: public BaseInterface, public ChildrenInterface, public Interface3D
{
public:
	static const InterfaceId interfaceId = 0x200;

	virtual bool soundPlay(Sound* sound) = 0;
	virtual bool soundUpdate(Sound* sound) = 0;
};



#endif
