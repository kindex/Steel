/*id*********************************************************
	File: objects/path.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Path. Метка, которя движется по траектории от объекта к объекту.
************************************************************/

#ifndef __GAME_TAG_H
#define __GAME_TAG_H

#include "../steel.h"
#include "game_obj.h"

struct PathTarget
{
	std::string id;
	float	speed, smooth;
};

/*
Path. Метка, которя движется по траектории от объекта к объекту.
*/
class TagPath: public GameObjSet
{
	int currentTarget;
	steel::vector<PathTarget> target;
	bool smooth;
	steel::time smoothStart, smoothLen;

public:
	bool	init(ScriptLine	&s);
	void	ProcessPhysic(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
};

#endif
