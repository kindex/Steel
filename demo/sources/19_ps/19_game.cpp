/*id*********************************************************
	Unit: Demo 19
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system testtype
 ************************************************************/

#include "19_game.h"
#include "particle_calculator.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

GamePS::GamePS():
    boundingModel(NULL),
    crosses(0),
	gravityBackup(v3(0,0,0))
{}

GamePS_SteelPhysic::GamePS_SteelPhysic():
	physicEngine(NULL)
{}


bool GamePS::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}

    Config* physicConfig = _conf.find("physic");
    if (physicConfig == NULL)
    {
        abort_init("game physic", "Cannot find physic config");
    }

    speedup = _conf.getf("speed", 1.0f);
    global_gravity = _conf.getv3("physic.global_gravity", v3(0.0f, 0.0f, -9.8f));

    boundingModel = loadGraphObject(_conf, "boundingModel");

	return true;
}

bool GamePS_SteelPhysic::init(Config& _conf, Input& _input)
{
    if (!GamePS::init(_conf, _input))
    {
        return false;
    }
	physicEngine = new PhysicEngine;
	physicEngine->init(*physicConfig);
	physicEngine->inject(world);

    return true;
}


void GamePS::process()
{
	GameFreeScene::process();
	
    if (boundingModel != NULL)
    {
        ParticleCalculator visitor(boundingModel);

        visitor.clear();
        world->traverse(visitor);

        crosses = visitor.cnt;
    }
}

void GamePS_SteelPhysic::process()
{
	GamePS::process();
	
	if (timeInfo.frameLength > EPSILON)
	{
        physicEngine->setSpeedup(speedup);
		physicEngine->process(timeInfo);
	}
}

void GamePS::handleEventKeyDown(const std::string& key)
{
	if (key == "1") speedup = 0.01f;
	else if (key == "2") speedup = 0.05f;
	else if (key == "3") speedup = 0.2f;
	else if (key == "4") speedup = 0.5f;
	else if (key == "5") speedup = 1;
	else if (key == "6") speedup = 2;
	else if (key == "7") speedup = 5;
	else if (key == "8") speedup = 20;
	else if (key == "9") speedup = 50;
	else if (key == "-") speedup /= 2;
	else if (key == "+") speedup *= 2;
    else
    {
        GameFreeScene::handleEventKeyDown(key);
    }
}

void GamePS_SteelPhysic::handleEventKeyDown(const std::string& key)
{
	if (key == "g")
	{
		v3 current = physicEngine->getGravity();
		physicEngine->setGravity(gravityBackup);
		gravityBackup = current;
	}
    else
    {
        GameFreeScene::handleEventKeyDown(key);
    }
}

std::string GamePS::getWindowCaption()
{
    return caption + " Crosses: " + IntToStr(crosses);
}
