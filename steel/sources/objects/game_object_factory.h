/*id*********************************************************
	File: objects/game_object_factory.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Создаватель игровый объектов при загрузке из конфигов
 ************************************************************/

#ifndef __OBJECTS__GAME_OBJECT_FACTORY_H
#define __OBJECTS__GAME_OBJECT_FACTORY_H

#include "../steel.h"
#include <string>
class GameObject;
class Config;
class ParticleAnimator;
class ParticleEmitter;
class ParticleRenderer;

class GameObjectFactory
{
public:
	virtual GameObject* createGameObject(IN const std::string& className) const;
	virtual ParticleAnimator* createParticleAnimator(IN const std::string& className) const;
	virtual ParticleEmitter* createParticleEmitter(const std::string& _class) const;
	virtual ParticleRenderer* createParticleRenderer(const std::string& _class) const;
};

extern GameObjectFactory* gameObjectFactory;

GameObject* createGameObject(Config* conf, OUT std::string* _class = NULL);


#endif
