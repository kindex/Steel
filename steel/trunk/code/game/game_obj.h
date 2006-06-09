/*id*********************************************************
    Unit: game
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
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

class GameObjDummy: public virtual GraphInterface, public virtual PhysicInterface
{
public:
	uid			getId()			{	return 0;		}
	aabb		getPFrame()		{	return getFrame(); }
	aabb		getFrame()		{	return aabb();	}
	bool		cleanup()		{	return true;	}
	bool		cleanupP()		{	return cleanup();	}
	Vertexes*	getVertexes()	{	return NULL;	}
	Vertexes*	getPVertexes()	{	return getVertexes(); }
	Triangles*	getTriangles()	{	return NULL;	}
	Config*		getPMaterial()	{	return NULL;	}
	Normals*	getNormals()	{	return NULL;	}
	Lights*		getLights()		{	return NULL;	}
	GLines*		getLines()		{	return NULL;	}
	FaceMaterials* getFaceMaterials()	{	return NULL;	}
	TexCoords*	getTexCoords(int texNumber)	{	return NULL;	}
	CollisionType::CollisionType getCollisionType() { return CollisionType::none; }
};

class GameObj;
typedef std::vector<GameObj*> GameObjList;

/*
Игровой объект, который может:
имеет положение
рисоваться в игре
двигаться
имень детей
быть прикреплённым к родителю
*/
class GameObj: public GameObjDummy
{
public:
	GameObj			*parent;
	std::vector<GameObj*> children;
	velocity	vel;
	coord		mass;
	ObjectPosition	position;
	std::string		name;
	ProcessKind::ProcessKind		processKind;
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
	PositionKind::PositionKind	getPositionKind(){	return positionKind;}
	ProcessKind::ProcessKind	getProcessKind() { return processKind; }

	void setPositionKind(const PositionKind::PositionKind newKind);

	void			setProcessKind(const ProcessKind::ProcessKind _kind) { processKind = _kind; }
	virtual	bool	init(ScriptLine	&s, ResCollection &res);
	GameObj *findChildren(std::string name)
	{
		if(tag.find(name) != tag.end()) return tag[name]; else return NULL;
	}

	std::string getName() { return name; }
	void setName(std::string _name) { name = _name;}

	void	trigger(PhysicInterface *object) {}

	ObjectPosition	getGlobalPosition();
	velocity	getGlobalVelocity();
	float		getGlobalScale();

	void	setPosition(ObjectPosition const &newPos) { position = newPos; } 

	void attach(GameObj *obj) 
	{ 
		parent = obj; 
	}
	
	void addChildren(GameObj *obj);
	GameObj *getChildren(std::string name);

	GraphInterfaceList getChildrens()
	{
		GraphInterfaceList a;
		for(std::vector<GameObj*>::iterator it = children.begin(); it != children.end(); it++)
			a.push_back(*it);
		return a;
	}
	PhysicInterfaceList getPChildrens()
	{
		PhysicInterfaceList a;
		for(std::vector<GameObj*>::iterator it = children.begin(); it != children.end(); it++)
			a.push_back(*it);
		return a;
	}
	GameObj *getParent() { return parent; }

	ObjectPosition		getPosition() {return position;}

	// скорость
	velocity	getVelocity() { return vel; }
	void	setVelocity(velocity const &v) {vel = v; }
	// масса
	coord	getMass(){return mass;}

	bool	getTarget(v3 &targetPoint, coord &speed) {return false;}
	void	setTargetReached() {}

	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) {}
	void	processGraph(v3	cameraEye, v3 cameraDirection) {}
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

	Lights* getLights()
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
