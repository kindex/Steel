
#include "../common/logger.h"
#include "../common/utils.h"
#include "game_obj.h"
#include "particle_system.h"

using namespace std;

bool GameGroup::load(string script, ResCollection *res)
{
	Script *s = (Script*)res->add(Res::script, script);
	if(!s)
	{
		alog.msg("error game res", "Cannot load script");
		return false;
	}

	alog.msg("game script", IntToStr(s->count()) + " Lines");
	for(int i=0; i<s->count(); i++)
	{
		GameObj *obj = NULL;

		if(s->count(i)<4) continue;
		string kind		= s->gets(i, 0);
		string parent	= s->gets(i, 1);
		string id		= s->gets(i, 2);
		string model	= s->gets(i, 3);

		bool gameobj = false, g = false, p = false;

		if(kind == "light")
		{
			obj = new GameLight;
			
			gameobj = true;
			g = true;
		}

		if(kind == "g" || kind == "solid" || kind == "f")
		{
			if(!res->add(Res::model, model)) return false;

			obj = new GameObjModel((Model*)res->getModel(model));

			obj->setMovable(kind != "solid");
			obj->setRotatable(kind != "solid");

			g = true;
			if(kind == "f" || kind == "solid")
				p = true;

			gameobj = true;
		}
		if(kind == "tag")
		{
			obj = new GameObjDummy();
			g = true;
			p = true;
			gameobj = true;
		}

		if(kind == "path")
		{
			obj = new GamePath();
			
			((GamePath*)obj)->setSpeed(s->getf(i, 3));

			obj->setPosition(tag[s->gets(i, 4)]->getPosition());

			for(int j=4; j<s->count(i); j++)
				((GamePath*)obj)->addTarget(s->gets(i, j));

			g = true;
			p = true;
		}

		if(kind == "include")
		{
			obj = new GameGroup();
			
			if(!((GameGroup*)obj)->load(model, res)) return false;

			g = true;
			p = true;
			gameobj = true;
		}

		if(kind == "sprite")
		{
			res->add(Res::config, "material/" + model);
			obj = new GameSprite(s->getf(i, 7), model);
			g = true;
			gameobj = true;
		}

		if(kind == "ps")
		{
			Config *c = (Config*)res->add(Res::config, "particle_system/" + model);
			if(!c)
			{
				alog.msg("error game res", "Cannot load particle system config");
				return false;
			}
			obj = new GameParticleSystem(c, res);
			g = true;
			gameobj = true;
		}

		if(id != "" && obj)
		{
			tag[id] = obj;
			obj->setName(id);
		}

		if(gameobj && obj)
			obj->setPosition(v3( s->getf(i, 4), s->getf(i, 5), s->getf(i, 6)));

		if(obj)
		if(parent == "")
		{
			if(g || p)	addChildren(obj);
			alog.msg("game script", string("Added object '") + kind + ":" + id + "' to global space");
		}
		else
		{
			if(tag.find(parent) == tag.end())
			{
				alog.msg("error game script", string("Object with id '") + parent + "' not found");
				return false;
			}
			tag[parent]->addChildren(obj);
			alog.msg("game script", string("Added object '") + kind + ":" + id + "' to '" + parent + "'");
		}
	}
	return true;
}


bool GamePath::getTarget(v3 &targetPoint, coord &_speed) 
{
	if(!target.empty())
	{
		if(parent == NULL) return false;
		GameObj *o;
		if(!(o = parent->getChildren(target[currentTarget]))) return false;

		targetPoint = o->getPosition();
		_speed = speed;
		return true;
	}
	else
		return false;
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

GameSprite::GameSprite(coord width, std::string material)
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

