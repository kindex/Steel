#ifndef __PARICLE_SYSTEM__EMITTER_H
#define __PARICLE_SYSTEM__EMITTER_H

#include "particle_system.h"

class SimpleEmitter: public ParticleEmitter
{
public:
	void born(Particle &particle); // ������� �������
	void process(steel::time curTime, steel::time frameLength, ModificationTime modificationTime);

};

#endif
