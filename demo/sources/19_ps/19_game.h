/*id*********************************************************
	Unit: Demo 19
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system testtype
 ************************************************************/

#ifndef _19_GAME_H_
#define _19_GAME_H_

#include "../game_free_scene.h"

#include <physic/physic_particle.h>

class GamePS: public GameFreeScene
{
public:
	GamePS();
	bool init(Config& _conf, Input& _input);
	void process();

private:
	PhysicEngine* physicEngine;
};

#endif
