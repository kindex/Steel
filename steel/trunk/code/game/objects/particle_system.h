
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "sprites.h"


struct Particle
{
	v3		position, velocity;
	float	lifetime;
};

class ParticleSystem: public Sprites
{
protected:
	std::vector<Particle> particle;
	Config	*conf;

public:
	ParticleSystem() {}
	bool init(ScriptLine	&s, ResCollection &res);

	void process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
	void processGraph(v3	cameraEye);
	void born(int partice_number);
	PositionKind	getPositionKind(){	return global;}
};


#endif
