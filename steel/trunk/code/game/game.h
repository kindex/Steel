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

class Game; // forward declaration, cross-use

#include "../input/input.h"

#include <string>
#include <map>

/*
Класс, задающий правила игры
*/
class Game: public steelAbstract
{
protected:
	double speed, time, moveSpeed;
	bool _alive;
//  Input-Output 
	ResCollection	*res;
	Input		*input;
	Config		*conf;

	void processKeyboard();

// World
	GameGroup	*world;
/*	std::vector<GraphInterface*> gobj;
	std::vector<PhysicInterface*> pobj;*/

// Camera
	v3	eye, direction;


public:
	Game() { } 
	bool init(ResCollection *_res, std::string _conf, Input *_input);

	void bind(GraphEngine *graph);
	void bind(PhysicEngine *physic);
	void draw(GraphEngine *graph);
	void process(PhysicEngine *physic, steel::time globalTime, steel::time time);

	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);

	void handleMouse(double dx, double dy);
	bool isAlive() {return _alive;} 
	void setspeed(double _speed, double _time) {speed = _speed; time = _time; } 

	v3	getGlobalPosition(std::string obj);
};

#endif
