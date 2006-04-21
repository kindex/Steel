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
������� ������, ������� �����:
����� ���������
���������� � ����
���������
����� �����
���� ������������ � ��������
*/
class GameObj: public GraphInterface, public PhysicInterface // Abstract
{
public:
	GameObj *parent;
	std::vector<GameObj*> children;
	v3 velocity;
	coord mass;
	matrix44 matrix;
	bool movable, rotatable;
	std::string name;

protected:
	std::map<std::string, GameObj*>	tag;

public:
	GameObj() 
	{
		movable = false;
		rotatable = false;
		parent = false;
	}
	std::string getName() { return name; }
	void setName(std::string _name) { name = _name;}

	void setMovable(bool _movable)
	{
		movable = _movable;
	}
	void setRotatable(bool _rotatable)
	{
		rotatable = _rotatable;
	}
	matrix44 getMatrix()	{		return matrix;	}
	matrix44 getPMatrix()	{		return matrix;	}
	void	setPMatrix(matrix44 const &m) { matrix = m; } 

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
	v3		getVelocity() { return velocity; }
	void	setVelocity(v3 const &v) {velocity = v; }
	// �����
	coord	getMass(){return mass;}

	bool	isMovable() { return movable; }
	bool	isRotatable(){ return rotatable; }

	bool	getTarget(v3 &targetPoint, coord &speed) {return false;}
	void	setTargetReached() {}

	void	process(steel::time speed) {}
};
/*
�������� ������, GraphMesh � PhysicMesh ������� �� ������
����� ����� �����.
*/
class GameObjModel: public GameObj
{
protected:
	Model *m;

public:
	void assignModel(Model *M)
	{
		m = M;
	}
	GameObjModel(Model *M) { assignModel(M);}
	aabb getFrame()	
	{		
		if(m)	return m->frame;
		else	return aabb();
	}
	aabb getPFrame()	
	{		
		if(m)	return m->frame;
		else	return aabb();
	}
	bool cleanup()	{	return true;		}
	bool cleanupP()	{	return cleanup();	}
	Vertexes*	getVertexes()
	{
		if(m)
			return &m->vertex;
		else
			return NULL;
	}
	Vertexes*	getPVertexes() { return getVertexes(); }
	Triangles*	getTriangles()
	{
		if(m)
			return &m->triangle;
		else
			return NULL;
	}
	Triangles*	getPTriangles() {	return getTriangles(); }

	MapCoords*	getMapCoords()
	{
		if(m)
			return &m->mapcoord;
		else
			return NULL;
	}
	Normals* getNormals()
	{
		if(m)
			return &m->normal;
		else
			return NULL;
	}
	FaceMaterials* getFaceMaterials()
	{
		if(m)
			return &m->faceMaterial;
		else
			return NULL;
	}
	Lights* getLights()
	{
		return NULL;
	}
	Sprites* getSprites() { return NULL; }
};

/*
Dummy. ������, ������� ����� ��������� � �����, �� �� ����� ����������� �����.
*/

class GameObjDummy: public GameObj
{
public:
	aabb		getPFrame()		{	return getFrame(); }
	aabb		getFrame()		{	return aabb();	}
	bool		cleanup()		{	return true;	}
	bool		cleanupP()		{	return cleanup();	}
	Vertexes*	getVertexes()	{	return NULL;	}
	Vertexes*	getPVertexes()	{	return getVertexes(); }
	Triangles*	getTriangles()	{	return NULL;	}
	Triangles*	getPTriangles() {	return getTriangles(); }
	MapCoords*	getMapCoords()	{	return NULL;	}
	Normals*	getNormals()	{	return NULL;	}
	Lights*		getLights()		{	return NULL;	}
	Sprites*	getSprites()	{	return NULL;	}
	FaceMaterials* getFaceMaterials()	{	return NULL;	}
};

/*
����� (tag). ������ ������ ������� ��� ������ �������� (����� �����)
*/
class GameTag: public GameObjDummy
{

};

/*
Path. �����, ������ �������� �� ���������� �� ������� � �������.
*/
class GamePath: public GameObjDummy
{
	int currentTarget;
	std::vector<std::string> target;
	coord speed;
public:
	GamePath() { currentTarget = 0; speed = 1.0; }
	void addTarget(std::string _target) { target.push_back(_target); }
	bool getTarget(v3 &targetPoint, coord &_speed);
	void setTargetReached()
	{
		if(!target.empty())
			currentTarget = (currentTarget + 1)%target.size();
	}
	void setSpeed(coord	_speed) { speed = _speed; }
};

/*
�������� ���������.
*/
class GameLight: public GameObjDummy
{

public:
	Lights* getLights()
	{
		Lights *a = new Lights(1);
		a->operator [](0).intensivity = 1.0f;
		a->operator [](0).range = 1000;
		return a;
	}
};

/*
��������� ��� ������ ��������.
��������� �� ������������ �� ������ �������� �
�������� ��������� �������.
*/

class GameDomain: public GameObjDummy
{

};

/*
��������� ��� ������ ��������.
� ���� ������������� ������ ������� � �������� ��� ������ �����.
���� �� ������-������ �� ����� �������� ����, ������� ���
���� �����, �� ������ ���������� �� ������ � ��������� ������ ������.

��� ����� (�������������� ������ ������ ���������, � �� ����������� �
������� ��������.
*/

class GameGroup: public GameObjDummy
{
public:
	bool load(std::string script, ResCollection *res);
};

/*
1 Sprite
*/
class GameSprite: public GameObjDummy
{
protected:
	Sprites sprites;
public:
	GameSprite(coord width, std::string material);
	Sprites*	getSprites();

};


#endif
