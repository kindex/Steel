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

#include "19_game.h"

GamePS::GamePS():
	physicEngine(NULL)
{}

bool GamePS::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	physicEngine = new PhysicEngine;

	physicEngine->inject(world);

    speedup = 1;

	return true;
}

void GamePS::process()
{
	GameFreeScene::process();
	
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
