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

bool GamePS::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
    if (!loadScene(conf->find("scene")))
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

void GamePS::process()
{
	GameFreeScene::process();
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

std::string GamePS::getWindowCaption()
{
    return caption + " Crosses: " + IntToStr(crosses);
}
