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

#include "../19_ps/19_game.h"

#include <objects/combiner/graph_object.h>

class NxActor;
class NxScene;

class GameAgeiatech: public GamePS
{
public:
	GameAgeiatech();
	~GameAgeiatech();
	void handleEventKeyDown(const std::string& key);
	bool init(Config& _conf, Input& _input);
	void process();

    friend class GraphObjectVisitor;

private:
    NxActor* createSurface(const GraphObject& object);

	NxScene*		scene;
};

#endif
