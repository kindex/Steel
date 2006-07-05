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

class GameObjDummy: public virtual GraphObject, public virtual PhysicObject
{
public:
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
typedef steel::vector<GameObj*> GameObjList;

/*
������� ������, ������� �����:
����� ���������
���������� � ����
���������
����� �����
���� ������������ � ��������
*/
class GameObj: public GameObjDummy
{
public:
	GameObj			*parent;
	
	PhysicObjectList physicChildren;
	steel::svector<GraphObject*> graphChildren;

	velocity	vel;
	coord		mass;
	ObjectPosition	position;
	std::string		name;
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
	GameObj *findChildren(std::string name)
	{
		if(tag.find(name) != tag.end()) return tag[name]; else return NULL;
	}

	std::string getName() { return name; }
	void setName(std::string _name) { name = _name;}

	void	trigger(PhysicObject *object) {}

	void	setPosition(ObjectPosition const &newPos) { position = newPos; } 

	void attach(GameObj *obj) 
	{ 
		parent = obj; 
	}
	
	void addChildren(GameObj *obj);

	PhysicObject* getPhysicChildren(int i)	{		return physicChildren[i];	}
	int getPhysicChildrenCount(void)	{		return physicChildren.size();	}

	GraphObjectList* getGraphChildrenList()	{		return &graphChildren;	}

	GameObj *getParent() { return parent; }

	ObjectPosition		getPosition() {return position;}

	// ��������
	velocity	getVelocity() { return vel; }
	void	setVelocity(velocity const &v) {vel = v; }
	// �����
	coord	getMass(){return mass;}

	bool	getTarget(v3 &targetPoint, coord &speed) {return false;}
	void	setTargetReached() {}

	void	process(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) {}
	void	processGraph(v3	cameraEye, v3 cameraDirection) {}
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

	Lights* getLights()
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
