
#ifndef GAME_H
#define GAME_H

#include "../steel.h"
#include "../graph/graph_engine.h"
#include "../graph/primiteves/res_model.h"
#include "../res/res.h"
#include <string>
#include <map>

class GameObj: public res_model
{
public:
	GameObj *parent;
	std::vector<GameObj*> children;
public:
	MATRIX4X4 pos;
	
	GameObj() {}
	GameObj(Model *M) { assignModel(M);}
	MATRIX4X4 getMatrix()
	{
		return pos;
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
};

class GameLight: public GameObj
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
	std::vector<GameObj*> obj;
// Camera
	v3	eye, direction, angle;

public:
	Game(ResCollection *_res) { res = _res;  } 
	bool init();
	void process();
	void draw(GraphEngine *graph);
	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);
	void handleMouse(double dx, double dy);
	bool alive() {return _alive;} 
	void setspeed(double _speed, double _time) {speed = _speed; time = _time; } 
};

#endif
