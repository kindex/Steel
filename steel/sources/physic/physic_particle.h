/*id*********************************************************
	File: physic/physic_particle.h
	Unit: physic engine
	Part of: Steel engine
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Физический джижок - Particle system
 ************************************************************/

#ifndef _PHYSIC_PARTICLE_H_
#define _PHYSIC_PARTICLE_H_
#include "../steel.h"
#include "physic_interface.h"


class PhysicEngine: public Engine, public PhysicParticleInterface
{
	struct ParticleShadow: public Shadow // множество треугольников одного материала
	{
		v3		position;
		v3		velocity;
		v3		force;
		Config*	config;

		ParticleShadow(Engine* aEngine): Shadow(aEngine) {}
	};

public:
	bool setCurrentObject(GameObject*);
	bool isSupportingInterface(IN const InterfaceId);
	void addChild(GameObject* child);
	void addChild(ParticleShadow& shadow, GameObject* child);
	void deleteChild(GameObject* child);
	void clearChildren();
	void		setPosition(IN const v3);
	v3			getPosition();
	void		setVelocity(IN const v3);
	v3			getVelocity();
	void		setConfig(Config&);

	Shadow* getShadowClass(GameObject* object);
	virtual bool inject(GameObject* object);
	virtual bool remove(GameObject* object);
	virtual bool process(IN const TimeInfo& info);
	bool clear();
	void prepare(ParticleShadow* shadow, GameObject* parent);
	ParticleShadow* getShadow(GameObject* object) { return static_cast<ParticleShadow*>(Engine::getShadow(object)); }
	ParticleShadow* getShadow(uid id) { return static_cast<ParticleShadow*>(Engine::getShadow(id)); }

	ParticleShadow* currentShadow;

	PhysicEngine();

protected:
	pvector<GameObject*>	objects;
	TimeInfo				timeInfo;
};


#endif
