/*id*********************************************************
	File: objects/ps/animator.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Обрабатывает движение частиц в системе.
************************************************************/
#ifndef __PARTICLE_SYSTEM__ANIMATOR_H
#define __PARTICLE_SYSTEM__ANIMATOR_H
#include "../../steel.h"

#include "particle_system.h"
#include "../../physic/physic_particle.h"

class UniParticle: public GameObject
{
public:
	UniParticle(Particle*, Config*);
	void setParticle(Particle *_particle) { particle = _particle; }

	bool InitFromConfig(Config&);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
	void process(IN const ProcessInfo& info);
	bool supportsInterface(IN OUT Engine&, IN const InterfaceId);
	void bindEngine(IN OUT Engine&, IN const InterfaceId);

private:
	Config*			conf;
	Particle*		particle;
	PhysicEngine*	engine;
};

class UniPSanimator: public ParticleAnimator
{
public:
	UniPSanimator();
	bool initParticles();
	bool InitFromConfig(Config&);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo& info);
	bool beforeInject(IN OUT Engine&, IN const InterfaceId id);
	void afterRemove(IN OUT Engine&, IN const InterfaceId id);
	void onParticleBorn(int index);
	void onParticleDie(int index) {}

private:
	pvector<UniParticle*>	children;
	PhysicEngine*			engine;
	Config*					particleConf;
};


class SimpleAnimator: public ParticleAnimator
{
public:
	bool initParticles();
	bool InitFromConfig(Config&) {return true;}
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id) {return false;}
	void process(IN const ProcessInfo& info) {return;}
	void onParticleBorn(int index) {}
	void onParticleDie(int index) {}
};


#endif
