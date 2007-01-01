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

#include <steel.h>

#include "demo.h"
#include <common/logger.h>
#include <common/utils.h>
#include <res/res_main.h>
#include <objects/scene.h>
#include <audio/openal_engine.h>

using namespace std;


bool Demo::init(Config *_conf, Input *_input)
{
	conf = _conf;
	if(!conf)
	{
		log_msg("error game res", "Cannot load game config");
		return false;
	}

	input = _input; 
	input->setGame(this);

	// Init world
	eye = conf->getv3("camera.eye", v3(1.0f, 1.0f, 1.0f));

	v3 target = conf->getv3("camera.target", v3(0.0f,0.0f,0.0f));

	direction = target-eye;
	direction.normalize();

	accSpeed = conf->getf("camera.acc", 50);
	brakeSpeed = conf->getf("camera.brakes", 200);

	moveSpeed.loadZero();

	Config *scene = conf->find("scene");
	if(scene == NULL)
	{
		error("game res", "Cannot find scene config");
		return false;
	}

	world = createGameObject(scene);
	if(world == NULL)
	{
		error("game", "Cannot init scene");
		return false;
	}
	
	light = new GameLight();
	light->InitFromConfig(resConfig.add("flashlight.conf"));
	light->enable();
	if(light != NULL)
	{
		light->setPosition(eye);
	}


// ******************* PHYSIC **************************

	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;

	return true;
}
