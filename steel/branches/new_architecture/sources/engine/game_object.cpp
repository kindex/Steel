/*id*********************************************************
	File: engine/game_object.cpp
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		GameObject protottype
		От этого класса наследуются все игровые объекты.
************************************************************/

#include "game_object.h"

#include "../common/logger.h"
#include "../common/utils.h"

IdGenerator objectIdGenerator;
//ModificationTime globalFrameNumber;

uid IdGenerator::genUid() 
{ 
	return freeId++;
}
