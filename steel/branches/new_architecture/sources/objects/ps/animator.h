/*id*********************************************************
	File: objects/ps/animator.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Обрабатывает движение частиц в системе.
************************************************************/
#ifndef __PARTICLE_SYSTEM__ANIMATOR_H
#define __PARTICLE_SYSTEM__ANIMATOR_H

#include "particle_system.h"

class UniParticle: public PhysicObject
{
	OldConfig *conf;
	Particle *particle;
public:

	UniParticle(Particle *_particle, OldConfig *_conf) { particle = _particle; conf = _conf;}
	void setParticle(Particle *_particle) { particle = _particle; }

	CollisionType getCollisionType() 
	{ 
		return COLLISION_PARTICLE; 
	}
	
	float getMass(void)
	{
		return conf->getf("mass");
	}

	PositionKind	getPositionKind(void){	return POSITION_GLOBAL;}
	OldConfig* getPMaterial(void) { return conf; }

	ObjectPosition getPosition(void)
	{ 
		ObjectPosition pos;
		pos.loadIdentity();
		pos.setTranslation(particle->position);
		return pos; 
	}

	velocity	getVelocity(void) { return velocity(particle->velocity,v3(0,0,0));}
	void		setVelocity(const velocity &v) { particle->velocity = v.translation;}
	void setPosition(ObjectPosition const &newPosition) {particle->position = newPosition.getTranslation(); }

	ProcessKind getProcessKind(void) { return PROCESS_NONE; }

	aabb		getPFrame(void)		{	return aabb(); } // TODO
	bool		cleanupP(void)		{	return true;	}
	Vertexes*	getPVertexes(void)	{	return NULL;}
	Triangles*	getTriangles(void)	{	return NULL;}
};

class UniPSanimator: public ParticleAnimator
{
	steel::svector<UniParticle*> children;

public:
	bool initParticles(void);
	int getPhysicChildrenCount(void) { return children.size(); }
	PhysicObject* getPhysicChildren(int i) { return children[i]; }

	ModificationTime getChildrenModificationTime(void) { return set->modificationTime; }

	ObjectPosition getPosition(void) { return particleSystem->getPosition(); }
	PositionKind getPositionKind(void) { return particleSystem->getPositionKind(); }
	CollisionType getCollisionType(void) { return COLLISION_NONE; }

	void setPosition(ObjectPosition const &newPosition) {}
	ProcessKind getProcessKind(void) { return PROCESS_NONE; }
	void	ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime);

	aabb		getPFrame(void)		{	return aabb(); } // TODO
	bool		cleanupP(void)		{	return true;	}
	Vertexes*	getPVertexes(void)	{	return NULL;}
	Triangles*	getTriangles(void)	{	return NULL;}
	OldConfig*		getPMaterial(void)	{	return NULL;}

	velocity	getVelocity(void) { return velocity(v3(0,0,0),v3(0,0,0));}
	void		setVelocity(const velocity &v) {}

	float	getMass(void) { return 1.0f;}
};

#endif
