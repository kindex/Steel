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
#include "../physic/physic_engine.h"	// Kane
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
	ProcessKind	processKind;
	PositionKind	positionKind;

protected:
	std::map<std::string, GameObj*>	tag;

public:
	GameObj()
	{
		parent	= false;
		processKind	= PROCESS_NONE;
		positionKind = POSITION_LOCAL;
	}

	void setPositionKind(PositionKind newKind) { positionKind = newKind; }
	PositionKind	getPositionKind(){	return positionKind;}
	ProcessKind	getProcessKind() { return processKind; }

	void			setProcessKind(const ProcessKind _kind) { processKind = _kind; }
	virtual	bool	init(ScriptLine	&s);

	void setPosition(ObjectPosition const &newPos) { position = newPos; } 

	void attach(GameObj *obj) 
	{ 
		parent = obj; 
	}
	
	void addChildren(GameObj *obj);

	int getPhysicChildrenCount(void)	{		return (int)physicChildren.size();	}
	PhysicObject* getPhysicChildren(int i)	{		return physicChildren[i];	}

	int getGraphChildrenCount(void)	{		return (int)graphChildren.size();	}
	GraphObject* getGraphChildren(int i)	{		return graphChildren[i];	}

	GameObj *getParent() { return parent; }

	ObjectPosition		getPosition() {return position;}

	// скорость
	velocity	getVelocity() { return vel; }
	void	setVelocity(velocity const &v) {vel = v; }
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
protected:
	Lights *lights;
public:
	GameLight()
	{
		lights = new Lights(1);
		lights->at(0).position.set(0.0f, 0.0f, 0.0f);
		lights->at(0).color.set(1.0f, 1.0f, 1.0f, 1.0f);
		lights->at(0).distance = 3;
		lights->at(0).intensivity = 1;
	}
	
	~GameLight()
	{
		delete lights;
	}

	Lights* getLights()
	{
		return lights;
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
