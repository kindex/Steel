/*id*********************************************************
	File: objects/ps/emitter.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Инициализация и утилизация частиц в системе
************************************************************/
#include "emitter.h"
#include "../../common/utils.h"

void SimpleEmitter::born(Particle &particle)
{
	particle.position = particleSystem->getPosition().getTranslation() + v3(frand(), frand(), frand())*conf->getf("position_dispersion");
	particle.velocity.loadZero();
	particle.size = conf->getf("particle_size");
}


void SimpleEmitter::ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime)
{
	if(frand() < 0.5 && set->particles.size() > 1) // delete particle
	{
		int dieId = (int)floor(frand()*set->particles.size()); // particle number

		delete set->particles[dieId];

		set->particles[dieId] = set->particles.back();
		set->particles.pop_back();

		particleSystem->setChildrenChangeTime(modificationTime);
	}

	if(frand() < 0.6)  // born particle
	{
		int bornId = set->particles.size();
		set->particles.resize(bornId + 1);

		set->particles[bornId] = new Particle;

		born(*set->particles[bornId]);

		particleSystem->setChildrenChangeTime(modificationTime);
	}
}
