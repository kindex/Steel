#include "../../common/logger.h"
#include "../../common/utils.h"

#include "group.h"
#include "custom_path.h"
#include "path.h"
#include "sprites.h"
#include "particle_system.h"
#include "triangle.h"

using namespace std;

#define CHECK_KIND(KIND, CLASS, PROCESS_KIND)	\
	if(kind == KIND) {							\
		obj	= new CLASS;						\
		obj->setProcessKind(PROCESS_KIND);		\
	}

bool GameGroup::init(ScriptLine	&s, ResCollection &res)
{
	if(s.count()<4) return false;
	if(!GameObj::init(s, res)) return false;
	return load(s.gets(3), &res);
}

bool GameGroup::load(string script, ResCollection *res)
{
	if(!parent) positionKind = global;

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

		// —тандартна€ строка настройки
		// KIND	parent	id	CONF	X,Y,Z	AngleX,Y,Z	Scale	Vx,Vy,Vz
		// KIND = (solid|custom|path|sprite|ps|include|rag)
		// CONF - файл с конфигом, дл€ solid это модель
		// XYZ (вектор) - смещение (default 0 0 0) 
		// Angle (вектор) - поворот вокруг осей X,Y,Z в радианах (default 0,0,0)
		// Scale - масштабирование (default 1)
		// Vxyz (вектор) - скорость (default 0 0 0) - толька дл€ uni

		// Solid model object, cannot move
		CHECK_KIND("solid", GameObjModel, ProcessKind::none);
		// Model object, move & collision detection
		CHECK_KIND("uni", GameObjModel, ProcessKind::uni);

		// Triangle
		CHECK_KIND("solid_trg", GameTriangleObj, ProcessKind::none);
		CHECK_KIND("uni_trg", GameTriangleObj, ProcessKind::uni);

		// метка в простнанстве, не рисуетс€
		CHECK_KIND("tag", GameTag, ProcessKind::none);
		//"ps"		parent	id	CONF	X	Y	Z	Angle	Scale
		CHECK_KIND("ps", ParticleSystem, ProcessKind::custom);
		//"include"	parent	id	CONF	X	Y	Z	Angle	Scale
		CHECK_KIND("include", GameGroup, ProcessKind::none);
		// "sprite"	parent	id	MATERIAL	X	Y	Z	ALIGN	Scale(Sprite Size)
		// ALIGN =  (z|camera|screen|X,Y,Z), default screen
		// z - спарйт выровнен вертикально (в локальной системе координат) и повЄрнут к камере
		// camera - смотрит врегда в центр камеры
		// screen - стороны выровнены по сторонам экрана
		// X,Y,Z - вектор, указывающий направление спрайта (не зависит от положени€ камеры)
		CHECK_KIND("sprite", Sprite, ProcessKind::none);
		
		// объект, который движетс€ по своим законам
		// TODO правила движени€ - дифур
		CHECK_KIND("custom", CustomPath, ProcessKind::custom);

		// путь по контрольным точкам
		//"path"		id	{tag	speed	smooth}
		// метка, котора€ движетс€ по зацикленной траектории по контрольным точкам
		// {tag	speed	smooth} - така€ тройка указывает каждую контрольную точку и может повтор€тьс€ много раз в строке
		// tag - id объекта
		// speed - скорость движени€ к следующей метке. ≈сли траектори€ не должны быть зацикленной, то можно указать скорость 0 - тогда движение остановитс€
		// smooth - радиус закруглени€ дл€ перехода к следующей метке, плано интерполируютс€ скорости
		CHECK_KIND("path", TagPath, ProcessKind::custom);


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
