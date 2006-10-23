/*id*********************************************************
	File: game/game.h
	Unit: game
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
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

#include "../audio/openal_engine.h"
#include "../audio/audio_engine.h"

/*
Класс, задающий правила игры
*/

class Game
{
public:
	// виртуальные обработчики клавиатуры и мышки
	virtual void handleEventKeyDown(std::string key) = 0;
	virtual void handleEventKeyUp(std::string key) = 0;
	virtual void handleMouse(double dx, double dy) = 0;
};

// класс для разработки и тестирования движка
class Steel: public Game
{
protected:
// Camera
	v3	eye, direction, cameraSpeed;
	v3 g;
	float speedup, speed;
	steel::time time;
	float accSpeed, brakeSpeed;
	v3 moveSpeed;

	bool _alive, paused;
	int framesToPass;
//  Input-Output 
	Input	*input;
	Config	*conf;

	GraphEngine *graphEngine;
//	PhysicEngineSteel *physicEngine;
	PhysicEngine *physicEngine;
	AudioEngine	 *audioEngine;

// World - custom objects
	GameGroup	*world;
	GraphHelper	*physicHelper;
//	Sprite		*crosshair;
	GameObj		*lightTag;
	GameLight	*light;

public:
	Steel(void):
	input(NULL),	conf(NULL),
	graphEngine(NULL),	physicEngine(NULL),	audioEngine(NULL),
	world(NULL),	physicHelper(NULL),	lightTag(NULL),	light(NULL)
	{ } 

	virtual bool init(Config *_conf, Input *_input, std::string params);
	virtual void deinit(void);

	void processKeyboard();
	bool executeScript(std::string script);
	bool executeCommand(std::string command);

	void bind(GraphEngine *engine);
	void bindPhysicEngine(void);
	void bindAudioEngine(AudioEngine *engine);
	void draw(GraphEngine *engine);

	void insonify(AudioEngine *engine);

	void process(steel::time globalTime, steel::time time);
//	int getCollisionCount() { return physicEngine->total.collisionCount; }

	bool isAlive() {return _alive;} 
	void setspeed(float _speed, steel::time _time) {speed = _speed; time = _time; } 
	bool createObject(void);

	v3	getGlobalPosition(std::string obj);

	virtual void handleEventKeyDown(std::string key);
	virtual void handleEventKeyUp(std::string key);

	virtual void handleMouse(double dx, double dy);
};

#endif
