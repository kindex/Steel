/*id*********************************************************
	File: engine/id_generator.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Уникальный идентификатор игрового объекта или ресурса
 ************************************************************/

#ifndef ENGINE__ID_GENERATOR_H
#define ENGINE__ID_GENERATOR_H

#include "../common/pvector.h"

typedef unsigned int uid;
typedef pvector<uid> uidVector;

class IdGenerator
{
protected:
	uid freeId;
public:
	IdGenerator() { freeId = 1; }
	uid genUid();
};

extern IdGenerator objectIdGenerator;

#endif
