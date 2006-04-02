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
#include "../res/res.h"

#include "game_obj.h"

#include "../physic/physic_engine.h"
#include "../graph/graph_engine.h"

#include <string>
#include <map>

/*
Класс, задающий правила игры
*/
class Game: public steelAbstract
{
private:
	double speed, time, moveSpeed;
	bool _alive;
//  Input-Output 
	ResCollection	*res;
	std::map<std::string, bool> keyPressed;


	void processKeyboard();

// World
	std::vector<GraphInterface*> gobj;
	std::vector<PhysicInterface*> pobj;
	
	std::map<std::string, GameObj*>	tag;

// Camera
	v3	eye, direction, angle;

	Config *conf;

public:
	Game() { } 
	bool init(ResCollection *_res, std::string _conf);
	void process();
	void processPhysic(PhysicEngine *physic);
	void draw(GraphEngine *graph);

	bool isPressed(std::string key);
	
	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);

	void handleMouse(double dx, double dy);
	bool isAlive() {return _alive;} 
	void setspeed(double _speed, double _time) {speed = _speed; time = _time; } 
};

#endif
