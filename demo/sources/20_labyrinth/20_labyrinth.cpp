/*id*********************************************************
	Unit: Demo 19
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system testtype
 ************************************************************/

#include "20_labyrinth.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

GameLabyrinth::GameLabyrinth():
	physicEngine(NULL)
{}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	physicEngine = new PhysicEngine;

    Config* physicConfig = _conf.find("physic");
    if (physicConfig == NULL)
    {
        abort_init("game physic", "Cannot find physic config");
    }
	physicEngine->init(*physicConfig);

	physicEngine->inject(world);

	return true;
}

void GameLabyrinth::process()
{
	GameFreeScene::process();
	
	if (timeInfo.frameLength > EPSILON)
	{
        physicEngine->setSpeedup(1.0);
		physicEngine->process(timeInfo);
	}
}

void GameLabyrinth::handleEventKeyDown(const std::string& key)
{
    GameFreeScene::handleEventKeyDown(key);
}

std::string GameLabyrinth::getWindowCaption()
{
    return  "Labyrinth FPS " + graphTimer.getfps_s();
}
