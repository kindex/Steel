#ifndef __PARTICLE_SYSTEM__ANIMATOR_H
#define __PARTICLE_SYSTEM__ANIMATOR_H

#include "particle_system.h"

class UniParticle: public PhysicObject
{
	Config *conf;
	Particle *particle;
public:

	UniParticle(Particle *_particle, Config *_conf) { particle = _particle; conf = _conf;}
	void setParticle(Particle *_particle) { particle = _particle; }

	CollisionType::CollisionType getCollisionType() 
	{ 
		return CollisionType::particle; 
	}
	
	float getMass()
	{
		return conf->getf("mass");
	}

	PositionKind::PositionKind	getPositionKind(){	return PositionKind::global;}
	Config* getPMaterial() { return conf; }

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


	ProcessKind::ProcessKind getProcessKind() { return ProcessKind::none; }


	aabb		getPFrame()		{	return aabb(); } // TODO
	bool		cleanupP()		{	return true;	}
	Vertexes*	getPVertexes()	{	return NULL;}
	Triangles*	getTriangles()	{	return NULL;}

	std::string getName() { return ""; }
};

class UniPSanimator: public ParticleAnimator
{
	steel::svector<UniParticle*> children;

public:
	bool initParticles();
	int getPhysicChildrenCount() { return children.size(); }
	PhysicObject* getPhysicChildren(int i) { return children[i]; }

	ModificationTime getChildrenModificationTime(void) { return set->modificationTime; }

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind::PositionKind getPositionKind() { return particleSystem->getPositionKind(); }
	CollisionType::CollisionType getCollisionType() { return CollisionType::none; }

	void setPosition(ObjectPosition const &newPosition) {}
	ProcessKind::ProcessKind getProcessKind() { return ProcessKind::none; }
	void	process(steel::time curTime, steel::time frameLength, ModificationTime modificationTime);

	aabb		getPFrame()		{	return aabb(); } // TODO
	bool		cleanupP()		{	return true;	}
	Vertexes*	getPVertexes()	{	return NULL;}
	Triangles*	getTriangles()	{	return NULL;}
	Config*		getPMaterial()	{	return NULL;}

	velocity	getVelocity() { return velocity(v3(0,0,0),v3(0,0,0));}
	void		setVelocity(const velocity &v) {}

	float	getMass() { return 1.0f;}
	std::string getName() { return ""; }
};

#endif
