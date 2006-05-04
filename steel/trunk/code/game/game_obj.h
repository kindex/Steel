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
Игровой объект, который может:
имеет положение
рисоваться в игре
двигаться
имень детей
быть прикреплённым к родителю
*/
class GameObj: public virtual GraphInterface, public virtual PhysicInterface // Abstract
{
public:
	GameObj			*parent;
	std::vector<GameObj*> children;
	v3			velocity;
	coord		mass;
	matrix44	matrix;
	bool movable, rotatable;
	std::string		name;
	ProcessKind::ProcessKind		processKind;
	PositionKind	positionKind;

protected:
	std::map<std::string, GameObj*>	tag;

public:
	GameObj()
	{
		movable = false;
		rotatable = false;
		parent	= false;
		processKind	= ProcessKind::none;
		positionKind = local;
	}
	PositionKind	getPositionKind(){	return positionKind;}
	ProcessKind::ProcessKind		getProcessKind() { return processKind; }
	void			setProcessKind(const ProcessKind::ProcessKind _kind) { processKind = _kind; }
	virtual	bool	init(ScriptLine	&s, ResCollection &res);
	GameObj *findChildren(std::string name)
	{
		if(tag.find(name) != tag.end()) return tag[name]; else return NULL;
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

	void	trigger(PhysicInterface *object) {}


	matrix44 getMatrix()	
	{		
		return matrix;	
	}
	matrix44	getGlobalMatrix();
	v3			getGlobalVelocity();


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

	// скорость
	v3		getVelocity() { return velocity; }
	void	setVelocity(v3 const &v) {velocity = v; }
	// масса
	coord	getMass(){return mass;}

	bool	isMovable() { return movable; }
	bool	isRotatable(){ return rotatable; }

	bool	getTarget(v3 &targetPoint, coord &speed) {return false;}
	void	setTargetReached() {}

	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) {}
	void	processGraph(v3	cameraEye, v3 cameraDirection) {}
};
/*
Рисуемый объект, GraphMesh и PhysicMesh берутся из модели
Может иметь детей.
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
	GameObjModel()
	{ 
		m = NULL;
	}
	bool	init(ScriptLine	&s, ResCollection &res);


	uid		getId() { return m->getId(); }
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
			return &m->triangleAll;
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

	MapCoords*	getMapCoords()
	{
		return m->getMapCoords();
	}
//	bool	init(ScriptLine	&line)

};

/*
Dummy. Обхект, который имеет положение и детей, но не имеет собственной формы.
*/

class GameObjDummy: public GameObj
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
	Normals*	getNormals()	{	return NULL;	}
	Lights*		getLights()		{	return NULL;	}
	FaceMaterials* getFaceMaterials()	{	return NULL;	}
	MapCoords*	getMapCoords()	{	return NULL;	}
};

/*
Метка (tag). Служит точкой отсчёта для других объектов (своих детей)
*/
class GameTag: public GameObjDummy
{
public:

};



/*
Источник освещения.
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
Контейнер для других объектов.
Разделяет всё пространство на домены коллизий и
отсекает невидимые объекты.
*/

/*class GameDomain: public GameObjDummy
{

};
*/


/*
1 Sprite
*/
/*class GameSprite: public GameObjDummy
{
protected:
	Sprites sprites;
public:
	GameSprite(coord width, std::string material);
	Sprites*	getSprites();
};
*/

#endif
