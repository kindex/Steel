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
public:
	GameLight(): light(NULL), engine(NULL), enabled(false) { position.loadIdentity(); }
	~GameLight()	{		delete light;	}

	bool isSuportingInterface(Engine& engine)
	{
		return engine.isSupportingInterface(INTERFACE_GRAPH);
	}
	bool beforeInject(IN OUT Engine&){return true;}
	void afterRemove(IN OUT Engine&);
	bool updateInformation(IN OUT Engine&);
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&){}
	bool InitFromConfig(IN Config&);

	void setPosition(const v3& _position) { position.setTranslation(_position); }
	void setPosition(const v3& position, 
					 const v3& direction,
					 const v3& up);
	void enable();
	void disable();
	void toggleEnable();

private:
	Light*			light;
	ObjectPosition	position;
	bool			enabled;
	GraphInterface*	engine;
};

#endif
