/*id*********************************************************
	File: objects/custom_path.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Path. Метка, скорость которой рассчитывает диффуром.
 ************************************************************/
#ifndef __GAME_CUSTOM_PATH_H
#define __GAME_CUSTOM_PATH_H

#include "../steel.h"
#include "game_obj.h"

/*
Path. Метка, скорость которой рассчитывает диффуром.
*/
class CustomPath: public GameObj
{
public:
	
	void	ProcessPhysic(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
};

#endif
