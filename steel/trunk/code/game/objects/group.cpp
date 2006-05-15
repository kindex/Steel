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
	conf = s.gets(3);
	return true;
}

bool GameGroup::load(ResCollection *res, GameObj *global)
{
	if(!parent) positionKind = Interface::global;

	Script *s = (Script*)res->add(Res::script, conf);
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

		// ����������� ������ ���������
		// KIND	parent	id	CONF	X,Y,Z	AngleX,Y,Z	Scale	Vx,Vy,Vz
		// KIND = (solid|custom|path|sprite|ps|include|rag)
		// CONF - ���� � ��������, ��� solid ��� ������
		// XYZ (������) - �������� (default 0 0 0) 
		// Angle (������) - ������� ������ ���� X,Y,Z � �������� (default 0,0,0)
		// Scale - ��������������� (default 1)
		// Vxyz (������) - �������� (default 0 0 0) - ������ ��� uni

		// Solid model object, cannot move
		CHECK_KIND("solid", GameObjModel, ProcessKind::none);
		// Model object, move & collision detection
		CHECK_KIND("uni", GameObjModel, ProcessKind::uni);

		// Triangle
		CHECK_KIND("solid_trg", GameTriangleObj, ProcessKind::none);
		CHECK_KIND("uni_trg", GameTriangleObj, ProcessKind::uni);

		// ����� � ������������, �� ��������
		CHECK_KIND("tag", GameTag, ProcessKind::none);
		//"ps"		parent	id	CONF	X	Y	Z	Angle	Scale
		CHECK_KIND("ps", ParticleSystem, ProcessKind::custom);
		//"include"	parent	id	CONF	X	Y	Z	Angle	Scale
		CHECK_KIND("include", GameGroup, ProcessKind::none);
		// "sprite"	parent	id	MATERIAL	X	Y	Z	ALIGN	Scale(Sprite Size)
		// ALIGN =  (z|camera|screen|X,Y,Z), default screen
		// z - ������ �������� ����������� (� ��������� ������� ���������) � ������� � ������
		// camera - ������� ������ � ����� ������
		// screen - ������� ��������� �� �������� ������
		// X,Y,Z - ������, ����������� ����������� ������� (�� ������� �� ��������� ������)
		CHECK_KIND("sprite", Sprite, ProcessKind::none);
		
		// ������, ������� �������� �� ����� �������
		// TODO ������� �������� - �����
		CHECK_KIND("custom", CustomPath, ProcessKind::custom);

		// ���� �� ����������� ������
		//"path"		id	{tag	speed	smooth}
		// �����, ������� �������� �� ����������� ���������� �� ����������� ������
		// {tag	speed	smooth} - ����� ������ ��������� ������ ����������� ����� � ����� ����������� ����� ��� � ������
		// tag - id �������
		// speed - �������� �������� � ��������� �����. ���� ���������� �� ������ ���� �����������, �� ����� ������� �������� 0 - ����� �������� �����������
		// smooth - ������ ����������� ��� �������� � ��������� �����, ����� ��������������� ��������
		CHECK_KIND("path", TagPath, ProcessKind::custom);


		if(obj)
		{
			if(obj->getProcessKind() == ProcessKind::uni)
			{
				if(obj->getPositionKind() == Interface::local)
				{
					obj->positionKind = Interface::global;

					matrix44 matrix = this->getGlobalMatrix();
					obj->setMatrix(matrix * obj->getMatrix());

					v3 vel = obj->getVelocity();

					obj->setVelocity(this->getGlobalVelocity() + (matrix*vel - matrix*v3(0,0,0)));
				}
				global->addChildren(obj);
			}
			else if(parent == "")
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
				obj->init(s->get(i), *res);

				alog.msg("game script error", string("Cannot init object ") + kind + ":" + id);
				return false;						
			}										
			if(kind == "include")
				if(!((GameGroup*)obj)->load(res, global)) return false;
		}
	}
	return true;
}

/*aabb GameGroup::getFrame()
{
	aabb box;
	box.clear();

	for(vector<GameObj*>::iterator it = children.begin(); it != children.end(); it++)
	{
		box.merge( (*it)->getFrame());
	}
	return box;
}*/

