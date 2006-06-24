/*id*********************************************************
    Unit: objects/path
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Path. Метка, которя движется по траектории от объекта к объекту.
************************************************************/

#ifndef __GAME_TAG_H
#define __GAME_TAG_H

#include "game_obj.h"

struct PathTarget
{
	std::string id;
	coord	speed, smooth;
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
	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
};

#endif
