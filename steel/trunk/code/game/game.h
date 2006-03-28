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
		�������� ������������� ������. � ���� ����� �������� ������� ����
		��� ����������� �����.
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
������� ������, ������� ����� ���������� � ����
*/
class GameObj: public PModel
{
public:
	GameObj *parent;
	std::vector<GameObj*> children;
	v3 position, velocity;
	coord mass;
	MATRIX4X4 matrix;
public:
	GameObj() {}
	GameObj(Model *M) { assignModel(M);}
	MATRIX4X4 getMatrix()
	{
		return matrix;
	}
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

	// ��������
	v3		getVelocity() { return velocity; }
	bool	setVelocity(v3 const &v) {velocity = v; return true; }
	// �����
	coord	getMass(){return mass;}
};

class GameSolidObj: public GameObj
{
public:
	GameSolidObj() {}
	GameSolidObj(Model *M) { assignModel(M);}
	bool	setVelocity(v3 const &v) {return false; }
	bool	setPosition(v3 const &v) {return false; }
};

class GameLight: public PDummy, public GDummy
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
�����, �������� ������� ����
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
