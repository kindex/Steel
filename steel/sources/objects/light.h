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

	bool supportsInterface(Engine& engine, IN const InterfaceId id)
	{
		return id == INTERFACE_GRAPH;
	}
	void afterRemove(IN OUT Engine&, IN const InterfaceId);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
	void bindEngine(IN OUT Engine&, IN const InterfaceId);
	void process(IN const ProcessInfo&){}
	bool InitFromConfig(IN Config&);
    Config* getConfig() const;

	void setPosition(const v3& _position) { position.setTranslation(_position); }
	void setPosition(const v3& position, 
					 const v3& direction,
					 const v3& up);
	void enable();
	void disable();
	void toggleEnable();
	const Light* getLight() const { return light; }

private:
	Light*			light;
	ObjectPosition	position;
	bool			enabled;
	GraphInterface*	engine;
};

#endif
