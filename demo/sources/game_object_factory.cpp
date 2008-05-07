/*id*********************************************************
	File: demo_game_object_factory.h
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Позволяет движку использовать классы, объявленные в Demo проекте.
 ************************************************************/
#include <steel.h>
#include "game_object_factory.h"
#include "objects/sphere.h"
#include "objects/tag.h"
#include "18_weapon/kar98k.h"
#include "20_labyrinth/character.h"
#include "23_ageia_tech/ageia_ps.h"

GameObject* DemoGameObjectFactory::createGameObject(IN const std::string& className) const
{
	if (className == "sphere")	return new Sphere;
	if (className == "kar98k")	return new Kar98k;
	if (className == "character")	return new Character;
	if (className == "tag")	return new Tag;

	return GameObjectFactory::createGameObject(className);
}

ParticleAnimator* DemoGameObjectFactory::createParticleAnimator(IN const std::string& className) const
{
	if (className == "AgeiaPsAnimator")	return new AgeiaPsAnimator;

	return GameObjectFactory::createParticleAnimator(className);
}

