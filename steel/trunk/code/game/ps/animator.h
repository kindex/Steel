#ifndef __PARTICLE_SYSTEM__ANIMATOR_H
#define __PARTICLE_SYSTEM__ANIMATOR_H

#include "particle_system.h"

class UniParticle: public PhysicInterface
{
	Config *conf;
	Particle *particle;
public:

	UniParticle(Particle *_particle, Config *_conf) { particle = _particle; conf = _conf;}

	CollisionType::CollisionType getCollisionType() 
	{ 
		return CollisionType::particle1; 
	}
	bool init(v3 emitterPosition, v3 emitterDirection, Config *conf);
	
	float getMass()
	{
		return conf->getf("mass");
	}

	PositionKind::PositionKind	getPositionKind(){	return PositionKind::global;}
	Config* getPMaterial() { return conf; }


	PhysicInterfaceList getPChildrens() { 		PhysicInterfaceList a;		return a; }

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


	ObjectPosition getGlobalPosition() { return getPosition(); }
	ProcessKind::ProcessKind getProcessKind() { return ProcessKind::none; }
	void setPositionKind(PositionKind::PositionKind const newPositionKind) {}


	aabb		getPFrame()		{	return aabb(); } // TODO
	bool		cleanupP()		{	return true;	}
	Vertexes*	getPVertexes()	{	return NULL;}
	Triangles*	getTriangles()	{	return NULL;}

	velocity	getGlobalVelocity() { return getVelocity(); }
	float		getGlobalScale() {return 1.0f;}

	std::string getName() { return ""; }

	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) {}
	void	trigger(PhysicInterface *object) {}



};

class UniPSanimator: public ParticleAnimator
{
	PhysicInterfaceList children;

public:
	bool initParticles();
	PhysicInterfaceList getPChildrens() { return children; }

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind::PositionKind getPositionKind() { return particleSystem->getPositionKind(); }
	CollisionType::CollisionType getCollisionType() { return CollisionType::none; }

	ObjectPosition getGlobalPosition() { return particleSystem->getGlobalPosition(); }
	void setPosition(ObjectPosition const &newPosition) {}
	ProcessKind::ProcessKind getProcessKind() { return ProcessKind::none; }
	void setPositionKind(PositionKind::PositionKind const newPositionKind) {}


	aabb		getPFrame()		{	return aabb(); } // TODO
	bool		cleanupP()		{	return true;	}
	Vertexes*	getPVertexes()	{	return NULL;}
	Triangles*	getTriangles()	{	return NULL;}
	Config*		getPMaterial()	{	return NULL;}

	velocity	getVelocity() { return velocity(v3(0,0,0),v3(0,0,0));}
	void		setVelocity(const velocity &v) {}
	velocity	getGlobalVelocity() { return getVelocity(); }
	float		getGlobalScale() {return 1.0f;}

	float	getMass() { return 1.0f;}
	std::string getName() { return ""; }

	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) {}
	void	trigger(PhysicInterface *object) {}
};

#endif
