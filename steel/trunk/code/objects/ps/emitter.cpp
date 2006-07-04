#include "emitter.h"
#include "../../common/utils.h"

void SimpleEmitter::born(Particle &particle)
{
	particle.position = particleSystem->getPosition().getTranslation() + v3(frand(), frand(), frand())*conf->getf("position_dispersion");
	particle.velocity.loadZero();
	particle.size = conf->getf("particle_size");
}


void SimpleEmitter::process(steel::time curTime, steel::time frameLength, ModificationTime modificationTime)
{
	if(frand() < 0.5 && set->particles.size() > 1) // delete particle
	{
		int n = (int)ceil(frand()*set->particles.size()); // particle number
		set->particles[n] = set->particles.back();
		set->particles.pop_back();
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
