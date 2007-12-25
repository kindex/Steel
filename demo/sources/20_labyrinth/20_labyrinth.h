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

#ifndef _20_LABYRINTH_H_
#define _20_LABYRINTH_H_

#include "../game_free_scene.h"
#include "labyrinth_generator.h"
#include "character.h"

#include <objects/combiner/graph_object.h>

typedef std::vector<GraphObject*> GraphObjectVector;

class NxPhysicsSDK;
class NxScene;

class GameLabyrinth: public GameFreeScene
{
public:
	GameLabyrinth();
	void handleEventKeyDown(const std::string& key);
	bool init(Config& _conf, Input& _input);
	void process();
    std::string getWindowCaption();
	void draw(GraphEngine&);

private:
	Labyrinth labyrinth;
	GraphObjectVector walls;
	ConfigArray* wscene[2];
	float length[2];
	int count[2];
    Character* character;
    v3 global_gravity;

// Physic
    bool initAgeia();
    void exitAgeia();
    NxActor* createSurface(const GraphObject& object);

	NxPhysicsSDK* physicsSDK;
	NxScene*      pScene;
};

#endif
