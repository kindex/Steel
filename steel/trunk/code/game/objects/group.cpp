#include "../../common/logger.h"
#include "../../common/utils.h"

#include "group.h"
#include "custom_path.h"
#include "path.h"
#include "sprites.h"
#include "particle_system.h"

using namespace std;

#define CHECK_KIND(KIND, CLASS, PROCESS_KIND)	\
	if(kind == KIND) {							\
		obj	= new CLASS;						\
		obj->setProcessKind(PROCESS_KIND);		\
	}


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

		if(s->count(i)<3) continue;
		string kind		= s->gets(i, 0);
		string parent	= s->gets(i, 1);
		string id		= s->gets(i, 2);

//		CHECK_KIND("light", GameLight, PhysicInterface::none);

		// Solid model object, cannot move
		CHECK_KIND("solid", GameObjModel, PhysicInterface::none);
		// объект, который движется по своим законам
		CHECK_KIND("custom", CustomPath, PhysicInterface::custom);
		// путь по контрольным точкам
		CHECK_KIND("path", TagPath, PhysicInterface::custom);

		// метка в простнанстве, не рисуется
		CHECK_KIND("tag", GameTag, PhysicInterface::none);


		CHECK_KIND("sprite", Sprites, PhysicInterface::none);
		
		CHECK_KIND("ps", ParticleSystem, PhysicInterface::custom);


		/*			gameobj = true;
			g = true;
		}
*/
/*		if(kind == "g" || kind == "solid" || kind == "f")
		{
			if(!res->add(Res::model, model)) return false;

			ProcessKind pkind = PhysicInterface::uni;
			if(kind == "solid")pkind = PhysicInterface::none;

			obj = new GameObjModel(local, pkind, (Model*)res->getModel(model));

			obj->setMovable(kind != "solid");
			obj->setRotatable(kind != "solid");

			g = true;
			if(kind == "f" || kind == "solid")
				p = true;

			gameobj = true;
		}
		if(kind == "tag")
		{
			obj = new GameTag(local, PhysicInterface::none);
			g = true;
			p = true;
			gameobj = true;
		}*/

/*		if(kind == "path")
		{
			obj = new GamePath();
			
			((GamePath*)obj)->setSpeed(s->getf(i, 3));

//			obj->setPosition(tag[s->gets(i, 4)]->getPosition());

			for(int j=4; j<s->count(i); j++)
				((GamePath*)obj)->addTarget(s->gets(i, j));

			g = true;
			p = true;
		}
*/
/*		if(kind == "dynpath")
		{
			obj = new CustomPath();
			
//			((GamePath*)obj)->setSpeed(s->getf(i, 3));

//			obj->setPosition(tag[s->gets(i, 4)]->getPosition());

//			for(int j=4; j<s->count(i); j++)
//				((GamePath*)obj)->addTarget(s->gets(i, j));

			g = true;
			p = true;
		}
*/

/*		if(kind == "include")
		{
			obj = new GameGroup(local, PhysicInterface::none);
			
			if(!((GameGroup*)obj)->load(model, res)) return false;

			g = true;
			p = true;
			gameobj = true;
		}
*/

/*		if(kind == "sprite")
		{
			res->add(Res::config, "material/" + model);
			obj = new GameSprite(s->getf(i, 7), model);
			g = true;
			gameobj = true;
		}
*/
/*		if(kind == "ps")
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
*/

		if(obj)
		if(parent == "")
		{
			addChildren(obj);
			alog.msg("game script", string("Added object '") + kind + ":" + id + "' to group space");
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
		if(id != "" && obj)
		{
			tag[id] = obj;
			obj->setName(id);
		}
		if(obj)
		{
			if(!obj->init(s->get(i), *res))			
			{										
				alog.msg("game script error", string("Cannot init object ") + kind + ":" + id);
				return false;						
			}										
		}
	}
	return true;
}
