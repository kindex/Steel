/*id*********************************************************
    Unit: game/objects/sprites
    Part of: Steel engine
    Version: 1.0
    Authors:
        * def [Alexander Papyshev]
    Description:
		Класс для анимирования графической системы частиц
************************************************************/

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "sprites.h"

struct ParticleSimple
{
	v3		position, velocity, acc;
	steel::time endTime, startTime;
	float	size, startSize, endSize, speedDown, lifeTime;
	bool	alive;

	void process(steel::time curTime, steel::time frameLength);
	void born(steel::time curTime, steel::time frameLength, Config *conf, matrix34 global, v3 globalVelocity);
};

//ParticleSystem nasleduet metod risovanija iz SpriteSystem
//Mozhno skazatj, 4to ParticleSystem zaniametsja Fizikoj PS
class ParticleSystem: public SpriteSystem
{
protected:
	TexCoords	texCoords1D; //dobavljaem teksturniju koordinatu dlja Alfa Kanala

	std::vector<ParticleSimple> particle;
	Config	*conf;

public:
	ParticleSystem() {}
	bool init(ScriptLine	&s, ResCollection &res);

	void process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
	void processGraph(v3	cameraEye, v3 cameraDirection);
	PositionKind::PositionKind	getPositionKind(){	return PositionKind::global; }

	TexCoords*	getTexCoords(int texNumber);
};


#endif
