/*id*********************************************************
    Unit: particle_system
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
		* kindex [Andrey Ivanov]
        * def [Alexander Papyshev]
    Description:
		������ ������ ������
************************************************************/

#ifndef __PARTICLE_SYSTEM_H
#define __PARTICLE_SYSTEM_H

#include "../game_obj.h"
#include <map>

class ParticleSystem;
class ParticleEmitter;

// ���� �������
struct Particle
{
	v3 position;
	v3 velocity;
	float size;
};

// ��������� ������
struct ParticleSet
{
	float countScale;
	ModificationTime modificationTime;

	steel::svector<Particle*> particles;

	friend class ParticleSystem;
};

class ParticleProcessor
{
protected:
	Config *conf;
	ParticleSet *set;
public:
};

// ����� ��� �������� ����� ������
class ParticleEmitter: public ParticleProcessor
{
protected:
	ParticleSystem *particleSystem;
public:
	virtual bool init(Config *_conf, ParticleSet *_set, ParticleSystem *_particleSystem);
	virtual void initParticles();
	virtual void born(Particle &particle) = 0; // ������� �������
	virtual void kill(int i); // ����� ������� � ������� i
	virtual void ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) = 0;
};

// ����� ��� ���������: ��������� ��������, ����� ��������, ����������
class ParticleRenderer: public GraphObject, public ParticleProcessor
{
protected:
	GraphObject *particleSystem;

public:
	virtual bool init(Config *_conf, ParticleSet *_set, GraphObject *_particleSystem);
	virtual bool initParticles() = 0;
};

// �����, ������� ��������� �������
class ParticleAnimator: public PhysicObject, public ParticleProcessor
{
protected:
	ParticleSystem *particleSystem;
public:
	virtual bool init(Config *_conf, ParticleSet *_set, ParticleSystem *_particleSystem);
	virtual bool initParticles() = 0;
};

// ������� �����
class ParticleSystem: public GameObj
{
	Config *conf, *emitterConf, *rendererConf, *animatorConf;
	ParticleSet			 particleSet;
	ParticleEmitter		*emitter;
	ParticleRenderer	*renderer;
	ParticleAnimator	*animator;

public:
	bool init(ScriptLine	&s);

	int GetPhysicChildrenCount(void) { return animator?1:0; }
	PhysicObject* getPhysicChildren(int i) { return animator; }

	int GetGraphChildrenCount(void) { return renderer?1:0; }
	GraphObject* GetGraphChildren(int i) { return renderer; }


	PositionKind::PositionKind getPositionKind(void) { return PositionKind::global;}

	void setChildrenChangeTime(ModificationTime time) 
	{ 
		particleSet.modificationTime = time;
	}

	ModificationTime getChildrenModificationTime(void) 
	{ 
		return 0;
	}

	void ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime)
	{
		emitter->ProcessPhysic(curTime, frameLength, modificationTime);
	}

};

#endif
