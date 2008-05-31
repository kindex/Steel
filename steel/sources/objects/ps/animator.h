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

class SimpleAnimator: public ParticleAnimator
{
public:
	bool initParticles();
	bool InitFromConfig(Config&);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo& info);
	void onParticleBorn(int index) {}
	void onParticleDie(int index) {}

private:
    v3 wind;
    float friction_k;
    float friction_power;
};


#endif
