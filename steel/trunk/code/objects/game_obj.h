/*id*********************************************************
    Unit: objects
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются 
		игровые объекты.
 ************************************************************/

#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "../physic/physic_interface.h"
#include "../graph/graph_interface.h"

#include "../res/model/model.h"
#include "../res/script/script.h"

/*
Dummy. Объект, который имеет положение и детей, но не имеет собственной формы.
*/


class GameObj;
typedef steel::vector<GameObj*> GameObjList;

/*
Игровой объект, который может:
имеет положение
рисоваться в игре
двигаться
имень детей
быть прикреплённым к родителю
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

	// скорость
	velocity	GetVelocity() { return vel; }
	void	SetVelocity(velocity const &v) {vel = v; }
	// масса
	coord	getMass(){return mass;}
};

class GameObjSet: public GameObj
{
public:
	aabb getPFrame();
};


/*
Метка (tag). Служит точкой отсчёта для других объектов (своих детей)
*/
class GameTag: public GameObj
{
public:

};


/*
Источник освещения.
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
Контейнер для других объектов.
Разделяет всё пространство на домены коллизий и
отсекает невидимые объекты.
*/
/*class GameDomain: public GameObjDummy
{
};
*/

#endif
