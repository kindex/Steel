/*id*********************************************************
    Unit: objects/group
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		��������� ��� ������ ��������.
		� ���� ������������� ������ ������� � �������� ��� ������ �����.
		���� �� ������-������ �� ����� �������� ����, ������� ���
		���� �����, �� ������ ���������� �� ������ � ��������� ������ ������.

		��� ����� (�������������� ������ ������ ���������, � �� ����������� �
		������� ��������.
 ************************************************************/

#include "../common/logger.h"
#include "../common/utils.h"

#include "group.h"
#include "custom_path.h"
#include "path.h"
#include "triangle.h"
#include "model_obj.h"
#include "ps/particle_system.h"
#include "../res/res_main.h"

using namespace std;

#define CHECK_KIND(KIND, CLASS, PROCESS_KIND)	\
	if(kind == KIND) {							\
		obj	= new CLASS;						\
		obj->setProcessKind(PROCESS_KIND);		\
	}

bool GameGroup::init(ScriptLine	&s)
{
	if(s.count()<4) return false;
	if(!GameObj::init(s)) return false;
	conf = s.gets(3);
	return true;
}

bool GameGroup::load(GameObj *global)
{
	if(!parent) positionKind = POSITION_GLOBAL;

	Script *s = resScript.add(conf);
	if(!s)
	{
		log_msg("error game res", "Cannot load script");
		return false;
	}

	log_msg("game script", IntToStr(s->count()) + " Lines");
	for(int i=0; i<s->count(); i++)
	{
		GameObj *obj = NULL;

		if(s->count(i)<3) continue;
		string kind		= s->gets(i, 0);
		string parent	= s->gets(i, 1);
		string id		= s->gets(i, 2);

//		CHECK_KIND("light", GameLight, PhysicObject::none);

		// ����������� ������ ���������
		// KIND	parent	id	CONF	X,Y,Z	AngleX,Y,Z	Scale	Vx,Vy,Vz
		// KIND = (solid|custom|path|sprite|ps|include|rag)
		// CONF - ���� � ��������, ��� solid ��� ������
		// XYZ (������) - �������� (default 0 0 0) 
		// Angle (������) - ������� ������ ���� X,Y,Z � �������� (default 0,0,0)
		// Scale - ��������������� (default 1)
		// Vxyz (������) - �������� (default 0 0 0) - ������ ��� uni

		// Solid model object, cannot move
		CHECK_KIND("solid", GameObjModel, PROCESS_NONE);
		// Model object, move & collision detection
		CHECK_KIND("uni", GameObjModel, PROCESS_UNI);

		// Triangle
		CHECK_KIND("solid_trg", GameTriangleObj, PROCESS_NONE);
		CHECK_KIND("uni_trg", GameTriangleObj, PROCESS_UNI);

		// ����� � ������������, �� ��������
		CHECK_KIND("tag", GameTag, PROCESS_NONE);
		// �������� ���������
		CHECK_KIND("light", GameLight, PROCESS_NONE);

		//"ps"		parent	id	CONF	X	Y	Z	Angle	Scale
		CHECK_KIND("ps", ParticleSystem, PROCESS_CUSTOM);
		
		//"include"	parent	id	CONF	X	Y	Z	Angle	Scale
		CHECK_KIND("include", GameGroup, PROCESS_NONE);
		// "sprite"	parent	id	MATERIAL	X	Y	Z	ALIGN	Scale(Sprite Size)
		// ALIGN =  (z|camera|screen|X,Y,Z), default screen
		// z - ������ �������� ����������� (� ��������� ������� ���������) � ������� � ������
		// camera - ������� ������ � ����� ������
		// screen - ������� ��������� �� �������� ������
		// X,Y,Z - ������, ����������� ����������� ������� (�� ������� �� ��������� ������)
//		CHECK_KIND("sprite", Sprite, PROCESS_NONE);
		
		// ������, ������� �������� �� ����� �������
		// TODO ������� �������� - �����
		CHECK_KIND("custom", CustomPath, PROCESS_CUSTOM);

		// ���� �� ����������� ������
		//"path"		id	{tag	speed	smooth}
		// �����, ������� �������� �� ����������� ���������� �� ����������� ������
		// {tag	speed	smooth} - ����� ������ ��������� ������ ����������� ����� � ����� ����������� ����� ��� � ������
		// tag - id �������
		// speed - �������� �������� � ��������� �����. ���� ���������� �� ������ ���� �����������, �� ����� ������� �������� 0 - ����� �������� �����������
		// smooth - ������ ����������� ��� �������� � ��������� �����, ����� ��������������� ��������
		CHECK_KIND("path", TagPath, PROCESS_CUSTOM);


		if(obj)
		{
			if(obj->getProcessKind() == PROCESS_UNI)
			{
				if(obj->getPositionKind() == POSITION_LOCAL)
				{ // TODO
					obj->positionKind = POSITION_GLOBAL;

//					ObjectPosition matrix = this->getGlobalPosition();
//					obj->setPosition(matrix * obj->getPosition());

					velocity vel = obj->getVelocity();
					
//					vel.translation = this->getGlobalVelocity().translation + (matrix*vel.translation - matrix*v3(0,0,0)); 
					vel.translation.loadZero();

					obj->setVelocity(vel);
				}
				global->addChildren(obj);
			}
			else if(parent == "")
			{
				addChildren(obj);
				log_msg("game script", string("Added object '") + kind + ":" + id + "' to group space");
			}
			else
			{
				if(tag.find(parent) == tag.end())
				{
					log_msg("error game script", string("Object with id '") + parent + "' not found");
					return false;
				}
				tag[parent]->addChildren(obj);
				log_msg("game script", string("Added object '") + kind + ":" + id + "' to '" + parent + "'");
			}
		}

		if(id != "" && obj)
		{
			tag[id] = obj;
//			obj->setName(id);
		}
		if(obj)
		{
			if(!obj->init(s->get(i)))
			{							
				obj->init(s->get(i));

				log_msg("game script error", string("Cannot init object ") + kind + ":" + id);
				return false;						
			}										
			if(kind == "include")
				if(!((GameGroup*)obj)->load(global)) return false;
		}
	}
	return true;
}
