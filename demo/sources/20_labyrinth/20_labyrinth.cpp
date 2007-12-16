/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Game main unit
 ************************************************************/


#include "20_labyrinth.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

GameLabyrinth::GameLabyrinth()
{}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}

	return true;
}

void GameLabyrinth::process()
{
	GameFreeScene::process();

}

void GameLabyrinth::handleEventKeyDown(const std::string& key)
{
    GameFreeScene::handleEventKeyDown(key);
}

std::string GameLabyrinth::getWindowCaption()
{
    return  "Labyrinth FPS " + graphTimer.getfps_s();
}
