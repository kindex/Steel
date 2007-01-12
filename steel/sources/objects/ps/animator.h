/*id*********************************************************
	File: objects/ps/animator.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Обрабатывает движение частиц в системе.
************************************************************/
#ifndef __PARTICLE_SYSTEM__ANIMATOR_H
#define __PARTICLE_SYSTEM__ANIMATOR_H

#include "particle_system.h"

class UniParticle: public GameObject
{
	Config *conf;
	Particle *particle;
public:

	UniParticle(Particle *_particle, Config *_conf) { particle = _particle; conf = _conf;}
	void setParticle(Particle *_particle) { particle = _particle; }

/*	CollisionType getCollisionType() 
	{ 
		return COLLISION_PARTICLE; 
	}
	
	float getMass()
	{
		return conf->getf("mass");
	}

	PositionKind	getPositionKind(){	return POSITION_GLOBAL;}
	OldConfig* getPMaterial() { return conf; }

	ObjectPosition getPosition()
	{ 
		ObjectPosition pos;
		pos.loadIdentity();
		pos.setTranslation(particle->position);
		return pos; 
	}

	velocity	getVelocity() { return velocity(particle->velocity,v3(0,0,0));}
	void		setVelocity(const velocity &v) { particle->velocity = v.translation;}
	void setPosition(ObjectPosition const &newPosition) {particle->position = newPosition.getTranslation(); }

	ProcessKind getProcessKind() { return PROCESS_NONE; }

	aabb		getPFrame()		{	return aabb(); } // TODO
	bool		cleanupP()		{	return true;	}
	Vertexes*	getPVertexes()	{	return NULL;}
	Triangles*	getTriangles()	{	return NULL;}*/
};

/*class UniPSanimator: public ParticleAnimator
{
	steel::svector<UniParticle*> children;

public:
	bool initParticles();

	int getPhysicChildrenCount() { return children.size(); }
	PhysicObject* getPhysicChildren(int i) { return children[i]; }

	ModificationTime getChildrenModificationTime() { return set->modificationTime; }

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind getPositionKind() { return particleSystem->getPositionKind(); }
	CollisionType getCollisionType() { return COLLISION_NONE; }

	void setPosition(ObjectPosition const &newPosition) {}
	ProcessKind getProcessKind() { return PROCESS_NONE; }
	void	ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime);

	velocity	getVelocity() { return velocity(v3(0,0,0),v3(0,0,0));}
	void		setVelocity(const velocity &v) {}

	float	getMass() { return 1.0f;}
};
*/

class SimpleAnimator: public ParticleAnimator
{
public:
	bool initParticles();
};


#endif
