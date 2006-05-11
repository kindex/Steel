#ifndef __GAME_TAG_H
#define __GAME_TAG_H

#include "../game_obj.h"

struct PathTarget
{
	std::string id;
	coord	speed, smooth;
};

/*
Path. Метка, которя движется по траектории от объекта к объекту.
*/
class TagPath: public GameObj
{
	int currentTarget;
	std::vector<PathTarget> target;
	bool smooth;
	steel::time smoothStart, smoothLen;

public:
	bool	init(ScriptLine	&s, ResCollection &res);
	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);

//	GamePath() { currentTarget = 0; speed = 1.0; }
//	void addTarget(std::string _target) { target.push_back(_target); }
//	bool getTarget(v3 &targetPoint, coord &_speed);
/*	void setTargetReached()
	{
		if(!target.empty())
			currentTarget = (currentTarget + 1)%target.size();
	}
	void setSpeed(coord	_speed) { speed = _speed; }
*/
};



#endif
