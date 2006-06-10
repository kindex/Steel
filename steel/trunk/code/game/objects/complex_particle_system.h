#ifndef __GAME_COMPLEX_PARTICLE_SYSTEM_H
#define __GAME_COMPLEX_PARTICLE_SYSTEM_H

#include "../game_obj.h"
#include "sprites.h"
#include <vector>

class Particle: public SpriteSystem
{
	Config *conf;
public:
	CollisionType::CollisionType getCollisionType() 
	{ 
		return CollisionType::particle1; 
	}
	bool init(ObjectPosition	emitterPosition, Config *conf, ResCollection &_res);
	coord	getMass(){return conf->getf("mass");}
	PositionKind::PositionKind	getPositionKind(){	return PositionKind::global;}
	Config* getPMaterial() { return conf; }

};

class ComplexParticleSystem: public GameObj
{
	Config *conf;
	std::vector<Particle*> particles;
public:
	bool init(ScriptLine	&s, ResCollection &res);
	GraphInterfaceList getChildrens();
	PhysicInterfaceList getPChildrens();

};


#endif

