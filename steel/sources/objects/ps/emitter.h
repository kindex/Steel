/*id*********************************************************
	File: objects/ps/emitter.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Инициализация и утилизация частиц в системе
************************************************************/
#ifndef __PARICLE_SYSTEM__EMITTER_H
#define __PARICLE_SYSTEM__EMITTER_H

#include "particle_system.h"

class SimpleEmitter : public ParticleEmitter // Random
{
public:
	void process(IN const ProcessInfo& info);
	bool updateInformation(IN OUT Engine&) {return false; }
	bool InitFromConfig(IN Config&);
	void born(Particle &particle, int index); // создать частицу
	void onParticleBorn(int index) {}
	void onParticleDie(int index) {}

enum EmitterType
{
    EMITTER_RANDOM_CUBE,
    EMITTER_RANDOM_SPHERE,
    EMITTER_RANDOM_FILLED_SPHERE,
    EMITTER_UNIFORM_CUBE,
    EMITTER_UNIFORM_SPHERE,
    EMITTER_REGULAR,
};

private:
    float       emitter_size;
    float       particle_size;
    float       emitter_type;
    int         init_size;
    EmitterType type;
};

#endif
