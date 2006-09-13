/*id*********************************************************
	Unit: Audio Interface
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
    Description:
		�������� ������.
 ************************************************************/

#ifndef __AUDIO_INTERFACE_H
#define __AUDIO_INTERFACE_H

#include "../interface.h"
#include "../res/audio/audio.h"

class AudioObject: public Interface
{
public:
	virtual bool AudioBeforeInject(void) { return true; }
	virtual int getAudioChildrenCount(void) 	{		return 0;	}
	virtual AudioObject* getAudioChildren(int i)	{		return NULL;	}

	virtual float getGain(void) = 0;
	virtual float getPitch(void) = 0;
	virtual bool isLooped(void) = 0;
	virtual Audio *getSound(void)  = 0;


/*
	getCoords();
	getMaterial();
	getAbsorptance();
	getDiffusion();
	get



--> to the game_obj.h
	class GameObj : public GraphObject, PhysicalObject, AudioObject
	{
	...
	AudioObjectList audioChildren;

	possibly add the following information:

	material
	absorptance
	diffusion

	and something like this

	};


*/
};

#endif