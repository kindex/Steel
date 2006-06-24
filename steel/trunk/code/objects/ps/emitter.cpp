#include "emitter.h"
#include "../../common/utils.h"

void SimpleEmitter::born(Particle &particle)
{
	particle.position = positionEmitter.getTranslation() + v3(frand(), frand(), frand())*conf->getf("position_dispersion");
	particle.velocity.loadZero();
	particle.size = conf->getf("particle_size");
}
