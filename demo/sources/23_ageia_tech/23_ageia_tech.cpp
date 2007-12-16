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

#include "23_ageia_tech.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

GameAgeiatech::GameAgeiatech()
{}

bool GameAgeiatech::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}

	return true;
}

void GameAgeiatech::process()
{
	GameFreeScene::process();

}

void GameAgeiatech::handleEventKeyDown(const std::string& key)
{
    GameFreeScene::handleEventKeyDown(key);
}

std::string GameAgeiatech::getWindowCaption()
{
    return  "Ageiatech FPS " + graphTimer.getfps_s();
}
