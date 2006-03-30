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
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
 ************************************************************/

#ifndef GAME_H
#define GAME_H

#include "../steel.h"
#include "../graph/graph_engine.h"
#include "../physic/physic_engine.h"
#include "../physic/physic_primitives.h"
#include "../res/res.h"
#include <string>
#include <map>

/*
Игровой объект, которым может рисоваться в игре
*/
class GameObj: public PModel
{
public:
	GameObj *parent;
	std::vector<GameObj*> children;
	v3 position, velocity;
	coord mass;
	matrix4 matrix;
public:
	GameObj() {}
	GameObj(Model *M) { assignModel(M);}
	matrix4 getMatrix()	{		return matrix;	}
	matrix4 getPMatrix()	{		return matrix;	}
	void	setPMatrix(matrix4 const &m) { matrix = m; } 


	void attach(GameObj *obj) 
	{ 
		parent = obj; 
	}
	void addChildren(GameObj *obj)
	{
		children.push_back(obj);
		obj->attach(this);
	}
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

	v3		getPosition() {return position;}
	bool	setPosition(v3 const &v)
	{ 
		position = v; 
		matrix.entries[12] = v.x;
		matrix.entries[13] = v.y;
		matrix.entries[14] = v.z;
		return true;
	}

	// скорость
	v3		getVelocity() { return velocity; }
	void	setVelocity(v3 const &v) {velocity = v; }
	// масса
	coord	getMass(){return mass;}

	bool	isMovable() { return true; }
	bool	isRotatable(){ return true; }

};

class GameSolidObj: public GameObj
{
public:
	GameSolidObj() {}
	GameSolidObj(Model *M) { assignModel(M);}

	bool	isMovable() { return false; }
	bool	isRotatable(){ return false; }
};

class GameLight: public PDummy, public GDummy
{
public:
	v3 position;
public:
	Lights* getLights()
	{
		Lights *a = new Lights(1);
		a->operator [](0).intensivity = 1.0f;
		a->operator [](0).range = 1000;
		return a;
	}

	matrix4 getMatrix()
	{
		matrix4 m;
		m.entries[12] = position.x;
		m.entries[13] = position.y;
		m.entries[14] = position.z;
		return m;
	}

	
};

/*
Класс, задающий правила игры
*/
class Game: public steelAbstract
{
private:
	double speed, time;
	bool _alive;
//  Input-Output 
	ResCollection *res;

	std::map<std::string, bool> keyPressed;
	bool isPressed(std::string key);
	void processKeyboard();

// World
	std::vector<GraphInterface*> gobj;
	std::vector<PhysicInterface*> pobj;
// Camera
	v3	eye, direction, angle;

	GameLight *light;

public:
	Game(ResCollection *_res) { res = _res;  } 
	bool init();
	void process();
	void processPhysic(PhysicEngine *physic);
	void draw(GraphEngine *graph);
	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);
	void handleMouse(double dx, double dy);
	bool alive() {return _alive;} 
	void setspeed(double _speed, double _time) {speed = _speed; time = _time; } 
};

#endif
