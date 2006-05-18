
#include "../common/logger.h"
#include "../common/utils.h"
#include "game_obj.h"

using namespace std;

void GameObj::changePositionKind(const PositionKind newKind)
{
	PositionKind oldKind = getPositionKind();
	if(oldKind == newKind) return;
	if(newKind == global && oldKind == local)
	{
		matrix = getGlobalMatrix();
		vel = getGlobalVelocity();
		positionKind = global;
	}
	else
	{} // TODO
}


bool GameObj::init(ScriptLine	&s, ResCollection &res)
{
	matrix.loadIdentity();

	matrix.setTranslation(s.getv3(4));

	v3 rot = s.getv3(5);

	matrix44 rx; rx.loadIdentity(); 	rx.setRotationX(rot.x);
	matrix44 ry; ry.loadIdentity(); 	ry.setRotationY(rot.y);
	matrix44 rz; rz.loadIdentity(); 	rz.setRotationZ(rot.z);

	float scalef = s.getf(6, 1.0f);
	if(scalef<=0) scalef = 1.0f;
	matrix44 scale; scale.loadIdentity(); 	scale.setScale(v3(scalef, scalef, scalef));

	matrix = scale*matrix*rx*ry*rz; // TODO: order

	setVelocity(velocity(s.getv3(7), s.getv3(8), s.getf(9)));

	return true;
}


matrix44	GameObj::getGlobalMatrix()
{
	if(getPositionKind() == Interface::global)
		return getMatrix();
	else 
	{
		GameObj *p = getParent();
		if(p)
			return p->getGlobalMatrix() * getMatrix();
		else
			return getMatrix();
	}
}

velocity GameObj::getGlobalVelocity()
{
	if(getPositionKind() == Interface::global)
		return getVelocity();
	else 
	{
		GameObj *p = getParent();
		if(p)
		{
			velocity v = getVelocity();
			velocity g = p->getGlobalVelocity();
			
			v.translation = g.translation + p->getGlobalMatrix()*v.translation;
			return v; // TODO
		}
		else
			return getVelocity();
	}
}

float GameObj::getGlobalScale()
{
	matrix44 global = getGlobalMatrix();
	v3 p;
    p = global*v3(1,0,0) - global*v3(0,0,0);
	return p.getLength();
}


void GameObj::addChildren(GameObj *obj)
{
	children.push_back(obj);
	obj->attach(this);
	string name = obj->getName();
	if(!name.empty())
	{
		if(tag.find(name) != tag.end())
		{
			alog.msg("error scene", string("Duplicated object id '") + name + "'");
		}
		tag[name] = obj;
	}
}

GameObj *GameObj::getChildren(std::string name)
{
	if(tag.find(name) == tag.end())
	{
		alog.msg("error scene", string("Object not found: '") + name + "'");
		return NULL;
	}
	return tag[name];
}

/*GameSprite::GameSprite(coord width, std::string material)
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

aabb GameObjSet::getPFrame()
{
	aabb box;
	box.clear();

	for(vector<GameObj*>::iterator it = children.begin(); it != children.end(); it++)
	{
		aabb loc = (*it)->getPFrame();
		if(!loc.empty())
		{
			loc.mul((*it)->getMatrix());
			box.merge(loc);
		}
	}
	return box;
}
