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

#include <objects/combiner/graph_object.h>

typedef std::vector<GraphObject*> GraphObjectVector;

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
	virtual v3 calulateCameraCollision(const v3& oldPos, const v3& newPos);

	Labyrinth labyrinth;
	GraphObjectVector walls;
	Config* rightWall;
	Config* downWall;
	float rightWallLength;
	float downWallLength;

	enum
	{
		C_FREE,
		C_FIRST_PERSON,
	}
	cameraMode;
};

#endif
