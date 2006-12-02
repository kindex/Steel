/*id*********************************************************
	File: objects/combiner/audio_object.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Физический аудио-объект.
 ************************************************************/

#ifndef __SIMPLE_SOUND_H
#define __SIMPLE_SOUND_H

#include "audio_object.h"

class SimpleSound: public AudioObject
{
public:
	bool InitFromConfig(Config *);
	void bindEngine(InterfaceId, Engine*);

protected:
	Sound* originalSound;		// must be vector		// ???
};

#endif
