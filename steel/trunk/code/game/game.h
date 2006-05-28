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
#include "objects/group.h"

#include "../physic/physic_engine_3d.h"
#include "../graph/graph_engine.h"
#include "../graph/graph_helper.h"

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
// Camera
	v3	eye, direction;
	v3 g;
	float speedup, speed;
	steel::time time;
	coord accSpeed, brakeSpeed;
	v3 moveSpeed;

	bool _alive, paused;
	int framesToPass;
//  Input-Output 
	ResCollection	*res;
	Input		*input;
	Config		*conf;

	GraphEngine *graphEngine;
	PhysicEngine3D *physicEngine;

	void processKeyboard();

// World - custom objects
	GameGroup	*world;
	GraphHelper	*physicHelper;

public:
	Game() { } 
	bool init(ResCollection *_res, std::string _conf, Input *_input);

	void bind(GraphEngine *engine);
	void bindPhysicEngine();
	void draw(GraphEngine *engine);
	void process(steel::time globalTime, steel::time time);
	int getCollisionCount() { return physicEngine->total.collisionCount; }

	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);

	void handleMouse(double dx, double dy);
	bool isAlive() {return _alive;} 
	void setspeed(float _speed, steel::time _time) {speed = _speed; time = _time; } 

	v3	getGlobalPosition(std::string obj);
};

#endif
