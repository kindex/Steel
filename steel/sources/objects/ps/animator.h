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
	bool InitFromConfig(Config&) {return true;}
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id) {return false;}
	void process(IN const ProcessInfo& info) {return;}
	void onParticleBorn(int index) {}
	void onParticleDie(int index) {}
};


#endif
