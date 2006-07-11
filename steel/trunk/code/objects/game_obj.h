/*id*********************************************************
    Unit: objects
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		���� ���� �������� ����������� � steel engine � ������
		�������� ������������� ������. � ���� ����� �������� 
		������� �������.
 ************************************************************/

#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "../physic/physic_interface.h"
#include "../graph/graph_interface.h"

#include "../res/model/model.h"
#include "../res/script/script.h"

/*
Dummy. ������, ������� ����� ��������� � �����, �� �� ����� ����������� �����.
*/


class GameObj;
typedef steel::vector<GameObj*> GameObjList;

/*
������� ������, ������� �����:
����� ���������
���������� � ����
���������
����� �����
���� ������������ � ��������
*/
class GameObj: public virtual GraphObject, public virtual PhysicObject
{
public:
	GameObj			*parent;
	
	PhysicObjectList physicChildren;
	GraphObjectList  graphChildren;

	velocity	vel;
	coord		mass;
	ObjectPosition	position;
	ProcessKind::ProcessKind	processKind;
	PositionKind::PositionKind	positionKind;

protected:
	std::map<std::string, GameObj*>	tag;

public:
	GameObj()
	{
		parent	= false;
		processKind	= ProcessKind::none;
		positionKind = PositionKind::local;
	}

	void setPositionKind(PositionKind::PositionKind newKind) { positionKind = newKind; }
	PositionKind::PositionKind	getPositionKind(){	return positionKind;}
	ProcessKind::ProcessKind	getProcessKind() { return processKind; }

	void			setProcessKind(const ProcessKind::ProcessKind _kind) { processKind = _kind; }
	virtual	bool	init(ScriptLine	&s);

	void SetPosition(ObjectPosition const &newPos) { position = newPos; } 

	void attach(GameObj *obj) 
	{ 
		parent = obj; 
	}
	
	void addChildren(GameObj *obj);

	int GetPhysicChildrenCount(void)	{		return physicChildren.size();	}
	PhysicObject* getPhysicChildren(int i)	{		return physicChildren[i];	}

	int GetGraphChildrenCount(void)	{		return graphChildren.size();	}
	GraphObject* GetGraphChildren(int i)	{		return graphChildren[i];	}

	GameObj *getParent() { return parent; }

	ObjectPosition		getPosition() {return position;}

	// ��������
	velocity	GetVelocity() { return vel; }
	void	SetVelocity(velocity const &v) {vel = v; }
	// �����
	coord	getMass(){return mass;}
};

class GameObjSet: public GameObj
{
public:
	aabb getPFrame();
};


/*
����� (tag). ������ ������ ������� ��� ������ �������� (����� �����)
*/
class GameTag: public GameObj
{
public:

};


/*
�������� ���������.
*/

class GameLight: public GameObj
{
public:

	Lights* GetLights()
	{
		Lights *a = new Lights(1);
		a->at(0).intensivity = 1.0f;
		a->at(0).range = 1000;
		return a;
	}
};

/*
��������� ��� ������ ��������.
��������� �� ������������ �� ������ �������� �
�������� ��������� �������.
*/
/*class GameDomain: public GameObjDummy
{
};
*/

#endif
