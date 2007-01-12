/*id*********************************************************
	File: objects/light.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Источник освещения.
 ************************************************************/
#ifndef OBJECTS__LIGHT_H
#define OBJECTS__LIGHT_H

#include "../steel.h"
#include "../graph/graph_interface.h"
#include "../engine/game_object.h"

class GameLight: public GameObject
{
protected:
	Light *light;
	ObjectPosition position;
	bool enabled;
	GraphInterface *engine;
public:
	GameLight(): light(NULL), engine(NULL), enabled(false) { position.loadIdentity(); }
	~GameLight()	{		delete light;	}

	bool InitFromConfig(Config *conf);
	bool isSuportingInterface(InterfaceId);
	void bindEngine(InterfaceId, Engine*);
	void afterRemove(InterfaceId, Engine*);
	bool updateInformation(InterfaceId id, Engine* engine);

	void setPosition(const v3 _position) { position.setTranslation(_position); }
	void enable();
	void disable();
	void toggleEnable();
};

#endif
