/*id*********************************************************
	Unit: Audio Interface
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.09.14 22:02:39] Kuzmi4: File Creation.
    Description:
		Звуковой движок.
 ************************************************************/

#ifndef __AUDIO_INTERFACE_H
#define __AUDIO_INTERFACE_H

#include "../engine.h"
#include "audio_engine.h"

class AudioObject;

typedef steel::svector<AudioObject*> AudioObjectList;

class AudioObject : public Interface
{
public:
	getSound() const { return sound; }
	getCoords() const { return coords; }
	setSound(Source newSound) { sound = newSound; }
	setCoords(v3 newCoords) { coords = newCoords; }
private:
	Source sound;
	v3 coords;


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