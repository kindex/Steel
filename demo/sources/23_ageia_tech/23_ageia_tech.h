/*id*********************************************************
	Unit: 23 Ageia tech
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Ageia test
 ************************************************************/

#ifndef _23_AGEIATECH_H_
#define _23_AGEIATECH_H_

#include "../game_free_scene.h"

#include <objects/combiner/graph_object.h>

class NxPhysicsSDK;
class NxScene;

class GameAgeiatech: public GameFreeScene
{
public:
	GameAgeiatech();
	~GameAgeiatech();
	void handleEventKeyDown(const std::string& key);
	bool init(Config& _conf, Input& _input);
	void process();
    std::string getWindowCaption();
	bool initAgeia();
	void exitAgeia();

private:
	NxPhysicsSDK*	physicsSDK;
	NxScene*		scene;
};

#endif
