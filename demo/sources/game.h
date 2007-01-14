/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
 ************************************************************/

#ifndef INTRO_H
#define INTRO_H

#include <steel.h>
#include <res/res.h>

#include <graph/graph_engine.h>
#include <audio/audio_engine.h>
#include <input/input.h>
#include "game_object_factory.h"

struct Spectator
{
	Camera	camera;
	v3		velocity;
};

class Game : public GameInput
{
public:
	virtual bool init(Config *_conf, Input *_input) = 0;
	virtual void deinit() = 0;
	virtual void process(IN const TimeInfo&) = 0;
	virtual bool isAlive() = 0;

	virtual GameObjectFactory* createGameObjectFactory() const = 0;

	virtual void bind(GraphEngine*) = 0;
	virtual void draw(GraphEngine*) = 0;

	virtual void bind(AudioEngine*) = 0;
	virtual void insonify(AudioEngine*) = 0;
};

#endif
