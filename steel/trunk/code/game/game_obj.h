/*id*********************************************************
    Unit: game
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        ������ ��� Division
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
};

class GameObj;
typedef std::vector<GameObj*> GameObjList;

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
	std::vector<GameObj*> children;
	velocity	vel;
	coord		mass;
	matrix44	matrix;
	std::string		name;
	ProcessKind::ProcessKind		processKind;
	PositionKind	positionKind;

protected:
	std::map<std::string, GameObj*>	tag;

public:
	GameObj()
	{
		parent	= false;
		processKind	= ProcessKind::none;
		positionKind = local;
	}
	PositionKind	getPositionKind(){	return positionKind;}
	ProcessKind::ProcessKind		getProcessKind() { return processKind; }

	void changePositionKind(const PositionKind newKind);

	void			setProcessKind(const ProcessKind::ProcessKind _kind) { processKind = _kind; }
	virtual	bool	init(ScriptLine	&s, ResCollection &res);
	GameObj *findChildren(std::string name)
	{
		if(tag.find(name) != tag.end()) return tag[name]; else return NULL;
	}

	std::string getName() { return name; }
	void setName(std::string _name) { name = _name;}

	void	trigger(PhysicInterface *object) {}

	matrix44 getMatrix()	
	{		
		return matrix;	
	}
	matrix44	getGlobalMatrix();
	velocity	getGlobalVelocity();
	float		getGlobalScale();

	void	setMatrix(matrix44 const &m) { matrix = m; } 

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

	v3		getPosition() {return v3(matrix.a[12], matrix.a[13], matrix.a[14]);}
	bool	setPosition(v3 const &v)
	{ 
		matrix.a[12] = v.x;
		matrix.a[13] = v.y;
		matrix.a[14] = v.z;
		return true;
	}

	// ��������
	velocity	getVelocity() { return vel; }
	void	setVelocity(velocity const &v) {vel = v; }
	// �����
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
����� (tag). ������ ������ ������� ��� ������ �������� (����� �����)
*/
class GameTag: public GameObj
{
public:

};


/*
�������� ���������.
*/
/*class GameLight: public GameObjDummy
{
public:

	Lights* getLights()
	{
		Lights *a = new Lights(1);
		a->operator [](0).intensivity = 1.0f;
		a->operator [](0).range = 1000;
		return a;
	}
};*/

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
