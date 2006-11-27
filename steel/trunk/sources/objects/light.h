#ifndef OBJECTS__LIGHT_H
#define OBJECTS__LIGHT_H

#include "../steel.h"

#include "../graph/graph_interface.h"
#include "../engine/game_object.h"

/*
Источник освещения.
*/

class GameLight: public GameObject
{
protected:
	Light *light;
	ObjectPosition position;
public:
	GameLight(void): light(NULL) {}
	~GameLight(void)	{		delete light;	}

	bool InitFromConfig(Config *conf);
	bool isSuportingInterface(InterfaceId);
	void bindEngine(InterfaceId, Engine*);
	bool updateInformation(InterfaceId id, Engine* engine);
};

#endif
