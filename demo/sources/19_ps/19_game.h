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

#ifndef _19_GAME_H_
#define _19_GAME_H_

#include "../game_free_scene.h"

#include <objects/combiner/graph_object.h>

class GamePS: public GameFreeScene
{
public:
	GamePS();
	void handleEventKeyDown(const std::string& key);
	bool init(Config& _conf, Input& _input);
	void process();
    std::string getWindowCaption();

protected:
	float         speedup;
    float         speed;
    GraphObject*  boundingModel;
    size_t			crosses;
	v3				global_gravity;
	v3				gravityBackup;
	Config*			physicConfig;
};


#endif
