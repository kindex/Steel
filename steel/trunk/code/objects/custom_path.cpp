/*id*********************************************************
	File: objects/custom_path.cpp
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

#include "../steel.h"
#include "custom_path.h"

void CustomPath::ProcessPhysic(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	v3 pos = getPosition().getVector();
	vel.translation = v3(-pos.y, pos.x, 0);
}
