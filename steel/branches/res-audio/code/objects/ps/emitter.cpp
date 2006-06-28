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
	if(frand() < -0.01)
	{
		set->particles.resize(set->particles.size() + 1);

		born(set->particles[set->particles.size() - 1]);

		particleSystem->setChildrenChangeTime(modificationTime);
	}
}
