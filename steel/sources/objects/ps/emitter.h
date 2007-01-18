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

class SimpleEmitter: public ParticleEmitter
{
public:
	void process(IN const ProcessInfo& info);
	bool updateInformation(IN OUT Engine&) {return false; }
	bool InitFromConfig(IN Config&);

	void born(Particle &particle); // создать частицу
};

#endif
