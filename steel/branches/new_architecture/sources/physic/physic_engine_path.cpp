/*id*********************************************************
	File: physic/physic_engine_path.cpp
	Unit: steel physic engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Steel Версия физического движка. 
		Отвечает за движение по траектории.
************************************************************/

#include "../steel.h"
#include "physic_engine_steel.h"

v3 PhysicPath::CalculateCoordinates(float time)
{
	// TODO

	return v3(0, 0, 0);
}

/*
Для инициализации должен быть проинициализирован
предок и первая цель.
*/
/*
bool TagPath::init(ScriptLine &s)
{
	if(!parent) return false;

	int cnt = (s.count()-3)/3;
	target.resize(cnt);
	for(int i=0; i<cnt; i++)
	{
		target[i].id = s.gets(3+i*3);
		if(target[i].id.empty()) return false;
		target[i].speed = s.getf(3+i*3 + 1, 0.0);
		target[i].smooth	= s.getf(3+i*3 + 2, 0.0);
	}
	currentTarget = 0;

//	GameObj *t = parent->findChildren(target[0].id);
//	if(!t) return false;
//	setPosition(t->getPosition());

	smooth = false;

	return true;
}

void TagPath::ProcessPhysic(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	int cnt = target.size();
	GameObj *t = parent->findChildren(target[currentTarget].id);
	if(!t) return;

	v3 curPos = getPosition().getVector();
	v3 tp = t->getPosition().getVector();

	float speed = target[currentTarget].speed;
	float dst = (tp - curPos).getLength();

	if(dst < frameLength*speed)
	{
		currentTarget = (currentTarget + 1)%cnt;
	}
	else 
	if(dst < target[currentTarget].smooth && !smooth)
	{
		smooth = true;
		smoothStart	= curTime;
		smoothLen = target[currentTarget].smooth / target[currentTarget].speed;
	}
	else
	if(smooth)
	{
		GameObj *n = parent->findChildren(target[(currentTarget + 1)%cnt].id);
		if(!n) return;
		float k = (float)((curTime - smoothStart) / smoothLen);

		if(k>=1)
		{
			smooth = false;
			currentTarget = (currentTarget + 1)%cnt;
		}
		else
			vel.translation = (tp-curPos).getNormalized()* speed * (1-k) +
			(n->getPosition().getVector()-curPos).getNormalized()* target[(currentTarget + 1)%cnt].speed * k;
	}
	else
		vel.translation = (tp-curPos).getNormalized()* speed;
		
}
*/
