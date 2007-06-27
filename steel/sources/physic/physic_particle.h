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
#include "../engine/engine_types.h"

class PhysicEngine: public Engine, public PhysicParticleInterface
{
	struct ParticleShadow: public Shadow // множество треугольников одного материала
	{
		v3		position;
		v3		velocity;
		v3		force;
		Config*	config;
		bool	enabled;

		float mass;
		float distance_k;
		
		float spring_r0;
		float spring_k;

		float gravity_k;
		float gravity_min_dist;
		float gravity_power;

		float friction_k;
		float friction_power;

		float lj_power1;
		float lj_power2;

		ParticleShadow(Engine* aEngine): Shadow(aEngine), enabled(false) {}
	};

public:
	bool setCurrentObject(GameObject*);
	bool isSupportingInterface(IN const InterfaceId);
	void addChild(GameObject* child);
	void addChild(ParticleShadow& shadow, GameObject* child);
	void deleteChild(GameObject* child);
	void clearChildren();
	void setPosition(IN const v3);
	v3	 getPosition();
	void setVelocity(IN const v3);
	v3	 getVelocity();
	void setConfig(Config&);
    void addTriangle(const Plane&);

	Shadow* getShadowClass(GameObject* object);
	virtual bool inject(GameObject* object);
	virtual bool remove(GameObject* object);
	virtual bool process(IN const TimeInfo& info);
    virtual void setSpeedup(float speedup);
	bool clear();
	void prepare(ParticleShadow* shadow, GameObject* parent);
	ParticleShadow* getShadow(GameObject* object) { return static_cast<ParticleShadow*>(Engine::getShadow(object)); }
	ParticleShadow* getShadow(uid id) { return static_cast<ParticleShadow*>(Engine::getShadow(id)); }

	ParticleShadow* currentShadow;

	PhysicEngine();

// ***************************** PARTICLE *************************
	v3 calculateForceForParticle(ParticleShadow* shadow1, ParticleShadow* shadow2);
	v3 PhysicEngine::calculateForceForParticle(ParticleShadow* shadow);
	bool PhysicEngine::processParticle(ParticleShadow* shadow);


protected:
	pvector<GameObject*>	objects;
	TimeInfo				timeInfo;
    pvector<Plane>			planes;
    float                   speedup;
};


#endif
