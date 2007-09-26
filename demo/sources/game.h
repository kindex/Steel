/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz@kindex.lv, http://kindex.lv]
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
	virtual bool init(Config &_conf, Input &_input) abstract;
	virtual void start() abstract;
	virtual void pause() abstract;
	virtual void unpause() abstract;
	virtual void deinit() abstract;
	virtual void process() abstract;
	virtual bool isAlive() abstract;

	virtual GameObjectFactory* createGameObjectFactory() const abstract;

	virtual void bind(GraphEngine&) abstract;
	virtual void draw(GraphEngine&) abstract;

	virtual void bind(AudioEngine&) abstract;
	virtual void insonify(AudioEngine&) abstract;
    virtual std::string getWindowCaption() abstract;
};

#endif
