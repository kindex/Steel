/*id*********************************************************
    Unit: game
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		���� ���� �������� ����������� � steel engine � ������
		�������� ������������� ������. � ���� ����� �������� ������� ����
		��� ����������� �����.
 ************************************************************/

#ifndef GAME_H
#define GAME_H

#include "../steel.h"
#include "../res/res.h"

#include "../objects/game_obj.h"
#include "../objects/group.h"

#include "../physic/physic_engine_steel.h"
#include "../graph/graph_engine.h"
#include "../graph/graph_helper.h"

class Game; // forward declaration, cross-use

#include "../input/input.h"

#include <string>
#include <map>

/*
�����, �������� ������� ����
*/
class Game: public steelAbstract
{
protected:
// Camera
	v3	eye, direction, cameraSpeed;
	v3 g;
	float speedup, speed;
	steel::time time;
	coord accSpeed, brakeSpeed;
	v3 moveSpeed;

	bool _alive, paused;
	int framesToPass;
//  Input-Output 
	Input		*input;
	Config		*conf;

	GraphEngine *graphEngine;
//	PhysicEngineSteel *physicEngine;
	PhysicEngine *physicEngine;

	void processKeyboard();

// World - custom objects
	GameGroup	*world;
	GraphHelper	*physicHelper;
//	Sprite		*crosshair;
	GameObj		*lightTag;
	GameLight	*light;

public:
	Game() { } 
	bool init(std::string _conf, Input *_input, std::string params);
	void deinit();

	bool executeScript(std::string script);
	bool executeCommand(std::string command);

	void bind(GraphEngine *engine);
	void bindPhysicEngine();
	void draw(GraphEngine *engine);
	void process(steel::time globalTime, steel::time time);
//	int getCollisionCount() { return physicEngine->total.collisionCount; }

	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);

	void handleMouse(double dx, double dy);
	bool isAlive() {return _alive;} 
	void setspeed(float _speed, steel::time _time) {speed = _speed; time = _time; } 
	bool createObject();

	v3	getGlobalPosition(std::string obj);
};

#endif
