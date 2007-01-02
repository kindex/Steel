/*id*********************************************************
	File: objects/simple_physic.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2007
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Физический аудио-объект.
 ************************************************************/

#ifndef __OBJECTS__SIMPLE_PHYSIC_H__
#define __OBJECTS__SIMPLE_PHYSIC_H__

#include "../steel.h"
#include "../engine/game_object.h"

class SimplePhysic: public GameObject
{
public:
	typedef enum
	{
		SIMPLE_PHYSIC_NONE,
		SIMPLE_PHYSIC_ROTATION,
		SIMPLE_PHYSIC_TRANSLATION,
		SIMPLE_PHYSIC_SCALE,
		SIMPLE_PHYSIC_PATH
	} SimplePhysicType;

	SimplePhysic();
	bool InitFromConfig(Config*);
	void bindEngine(InterfaceId, Engine*);
	void process(IN const ProcessInfo& info);
	bool isSuportingInterface(InterfaceId);
	bool updateInformation(InterfaceId, Engine*);

protected:
	GameObject* object;
	ObjectPosition startPosition;
	ObjectPosition currentPosition;
	SimplePhysicType type;
	float speed;
	v3 rotationAxis;
};

#endif
