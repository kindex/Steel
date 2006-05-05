
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "sprites.h"


struct Particle
{
	v3		position, velocity, acc;
	steel::time endTime, startTime;
	float	size, startSize, endSize, speedDown, lifeTime;
	bool	alive;

	void process(steel::time curTime, steel::time frameLength);
	void born(steel::time curTime, steel::time frameLength, Config *conf, matrix44 global, v3 globalVelocity);
};

//ParticleSystem nasleduet metod risovanija iz SpriteSystem
//Mozhno skazatj, 4to ParticleSystem zaniametsja Fizikoj PS
class ParticleSystem: public SpriteSystem
{
protected:
	MapCoords	mapCoords1D; //dobavljaem teksturniju koordinatu dlja Alfa Kanala

	std::vector<Particle> particle;
	Config	*conf;

public:
	ParticleSystem() {}
	bool init(ScriptLine	&s, ResCollection &res);

	void process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
	void processGraph(v3	cameraEye, v3 cameraDirection);
	PositionKind	getPositionKind(){	return global;}

	MapCoords*	getMapCoords(int mapNumber);
};


#endif
