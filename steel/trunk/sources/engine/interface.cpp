/*id*********************************************************
	File: interface.cpp
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Предок для интерфейтов объектов, которые имеют положение
		(глобальную, локальную систему координат)
************************************************************/

#include "interface.h"

#include "../common/logger.h"
#include "../common/utils.h"

IdGenerator objectIdGenerator;
ModificationTime globalFrameNumber;

uid IdGenerator::genUid() 
{ 
	return freeId++;
}
