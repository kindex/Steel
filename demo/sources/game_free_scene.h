/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz@kindex.lv, http://kindex.lv]
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
#include <common/timer.h>

class GameFreeScene : public Game
{
public:
	GameFreeScene();
	bool init(Config& _conf, Input& _input);
	void start();
	void pause();
	void unpause();
	void deinit();
	void process();

	GameObjectFactory* createGameObjectFactory() const;

	void bind(GraphEngine&);
	void draw(GraphEngine&);

	void bind(AudioEngine&);
	void insonify(AudioEngine&);
	bool isAlive() {return _alive;} 

	void handleEventKeyDown(const std::string& key);
	void handleEventKeyUp(const std::string& key);
	void handleMouse(double dx, double dy);

	void updatePhysicTime();


	//////////
	void processKeyboard();

protected:
	Config*			conf;
	Input*			input;
	Spectator		spectator;
	Spectator		flashlightPosition;
	GraphEngine*	graphEngine;
	AudioEngine*	audioEngine;
 	GameObject*		world;
 	GameLight*		flashlight;
	ProcessInfo		info;
	Timer			graphTimer;
	Timer			audioTimer;
	Timer			physicTimer;
	Timer			infoTimer;
	TimeInfo		timeInfo;

	float speedup, speed;
	float accSpeed, brakeSpeed;

	bool _alive, paused, firstframe;
	int framesToPass;
};

#endif
