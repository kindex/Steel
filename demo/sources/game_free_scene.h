/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для разработки и тестирования движка.
		Сцена загружается из файла. Свободно летающая камера.
 ************************************************************/

#ifndef _GAME_FREE_SCENE_H_
#define _GAME_FREE_SCENE_H_

#include "game.h"
#include <objects/light.h>

class GameFreeScene : public Game
{
public:
	GameFreeScene();
	bool init(Config *_conf, Input *_input);
	void deinit();
	void process(IN const TimeInfo&);

	GameObjectFactory* createGameObjectFactory() const;

	void bind(GraphEngine*);
	void draw(GraphEngine*);

	void bind(AudioEngine*);
	void insonify(AudioEngine*);
	bool isAlive() {return _alive;} 

	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);
	void handleMouse(double dx, double dy);


	//////////
	void processKeyboard();

protected:
	Config*			conf;
	Input*			input;
	Spectator		spectator;
	GraphEngine*	graphEngine;
	AudioEngine*	audioEngine;
 	GameObject*		world;
 	GameLight*		light;
	ProcessInfo		info;

	float speedup, speed;
	float accSpeed, brakeSpeed;

	bool _alive, paused;
	int framesToPass;
};

#endif
