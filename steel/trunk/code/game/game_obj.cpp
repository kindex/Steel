
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
		velocity = getGlobalVelocity();
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

	matrix = matrix*rx*ry*rz;

	float scale = s.getf(6, 1.0f);
	if(scale<=0) scale = 1.0f;
	
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			matrix.a[i+j*4] *= scale;

	setVelocity(s.getv3(7));

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

v3	GameObj::getGlobalVelocity()
{
	if(getPositionKind() == Interface::global)
		return getVelocity();
	else 
	{
		GameObj *p = getParent();
		if(p)
			return p->getGlobalMatrix() * getVelocity();
		else
			return getVelocity();
	}
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

bool	GameObjModel::init(ScriptLine	&s, ResCollection &res)
{
	if(!GameObj::init(s, res)) return false;
	
	if(s.count()<3) return false;
	m = (Model*)res.add(Res::model, s.gets(3));

	mass = 1;

	return m != NULL;
}
