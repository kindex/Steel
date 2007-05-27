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

	return true;
}

void GamePS::process()
{
	GameFreeScene::process();
	
	if (timeInfo.frameLength > EPSILON)
	{
		physicEngine->process(timeInfo);
	}
}

