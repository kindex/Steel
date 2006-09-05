/*id*********************************************************
	File: objects/game_obj.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются 
		игровые объекты.
 ************************************************************/

#include "../steel.h"
#include "../common/logger.h"
#include "../common/utils.h"
#include "game_obj.h"
using namespace std;

/*void GameObj::setPositionKind(const PositionKind newKind)
{
	PositionKind oldKind = getPositionKind();
	if(oldKind == newKind) return;
	if(newKind == POSITION_GLOBAL && oldKind == POSITION_LOCAL)
	{
//		position = getGlobalPosition();
		vel = getGlobalVelocity();
		positionKind = POSITION_GLOBAL;
	}
	else
	{} // TODO
}*/


bool GameObj::init(ScriptLine	&s)
{
	position.loadIdentity();

	position.setTranslation(s.getv3(4));

	v3 rot = s.getv3(5);

	matrix34 rx; rx.loadIdentity(); 	rx.setRotationX(rot.x);
	matrix34 ry; ry.loadIdentity(); 	ry.setRotationY(rot.y);
	matrix34 rz; rz.loadIdentity(); 	rz.setRotationZ(rot.z);

	float scalef = s.getf(6, 1.0f);
	if(scalef<=0) scalef = 1.0f;
	matrix34 scale; scale.loadIdentity(); 	scale.setScale(v3(scalef, scalef, scalef));

	position = position*rx*ry*rz*scale; // TODO: order

	setVelocity(velocity(s.getv3(7), s.getv3(8).getNormalized()*s.getf(9)));

	return true;
}


/*ObjectPosition	GameObj::getGlobalPosition()
{
	if(getPositionKind() == POSITION_GLOBAL)
		return getPosition();
	else 
	{
		GameObj *p = getParent();
		if(p)
			return p->getGlobalPosition() * getPosition();
		else
			return getPosition();
	}
}*/

/*velocity GameObj::getGlobalVelocity()
{
	if(getPositionKind() == POSITION_GLOBAL)
		return getVelocity();
	else 
	{
		GameObj *p = getParent();
		if(p)
		{
			velocity v = getVelocity();
			velocity g = p->getGlobalVelocity();
			
			v.translation = g.translation + p->getGlobalPosition().getMatrix33()*v.translation;
			return v; // TODO
		}
		else
			return getVelocity();
	}
}*/

/*float GameObj::getGlobalScale()
{
	matrix33 global = getGlobalPosition().getMatrix33();
	v3 p;
    p = global*v3(1,0,0);
	return p.getLength();
}*/


void GameObj::addChildren(GameObj *obj)
{
	graphChildren.push_back((GraphObject*)obj);
	physicChildren.push_back((PhysicObject*)obj);

	obj->attach(this);
//	string name = obj->getName();
/*	if(!name.empty())
	{
		if(tag.find(name) != tag.end())
		{
			log_msg("error scene", string("Duplicated object id '") + name + "'");
		}
		tag[name] = obj;
	}*/
}


/*GameSprite::GameSprite(float width, std::string material)
{
	sprites.resize(1);
	sprites[0].pos		= v3(0, 0, 0);
	sprites[0].width	= width;
	sprites[0].material	= material;
}

Sprites*	GameSprite::getSprites()
{
	return &sprites;
}
*/

/*aabb GameObjSet::getPFrame()
{
	aabb box;
	box.clear();

	for(PhysicObjectList::iterator it = physicChildren.begin(); it != physicChildren.end(); it++)
	{
		aabb loc = (*it)->getPFrame();
		if(!loc.empty())
		{
			loc.mul((*it)->getPosition());
			box.merge(loc);
		}
	}
	return box;
}*/
