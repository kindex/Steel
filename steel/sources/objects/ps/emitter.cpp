/*id*********************************************************
	File: objects/ps/emitter.cpp
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
#include "emitter.h"
#include "../../common/utils.h"


void SimpleEmitter::born(Particle &particle)
{
	particle.position = this->position + v3(frand(), frand(), frand())*conf->getf("position_dispersion", 1.0f);
	
	particle.velocity.loadZero();
	particle.size = conf->getf("particle_size", 1.0f);
}


void SimpleEmitter::process(IN const ProcessInfo& info)
{
	if(frand() < 0.5 && set->particles.size() > 1) // delete particle
	{
		int dieId = rand()%set->particles.size(); // particle number

		delete set->particles[dieId];

		if((size_t)(dieId + 1) < set->particles.size())
			set->particles[dieId] = set->particles.back();
		set->particles.pop_back();
	}

	if(frand() < 0.6f)  // born particle
	{
		int bornId = set->particles.size();
		set->particles.resize(bornId + 1);

		set->particles[bornId] = new Particle;

		born(*set->particles[bornId]);
	}
}

bool SimpleEmitter::InitFromConfig(Config *_conf)
{
	conf = _conf;
	position = conf->getv3("origin");

	return true;
}
