
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "sprites.h"


struct Particle
{
	v3		position, velocity, acc;
	float	endTime, startTime, size, speedDown;
	bool	alive;

	void process(steel::time curTime, steel::time frameLength);
	void born(steel::time curTime, steel::time frameLength, Config *conf, matrix44 global, v3 globalVelocity);
};

class ParticleSystem: public SpriteSystem
{
protected:
	std::vector<Particle> particle;
	Config	*conf;

public:
	ParticleSystem() {}
	bool init(ScriptLine	&s, ResCollection &res);

	void process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
	void processGraph(v3	cameraEye, v3 cameraDirection);
	PositionKind	getPositionKind(){	return global;}
};


#endif
