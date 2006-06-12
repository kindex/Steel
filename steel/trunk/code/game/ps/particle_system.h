/*id*********************************************************
    Unit: particle_system
    Part of: Steel engine
    Version: 1.0
    Authors:
		* kindex [Andrey Ivanov]
        * def [Alexander Papyshev]
    Description:
		Основа систем частиц
************************************************************/

#ifndef __PARTICLE_SYSTEM_H
#define __PARTICLE_SYSTEM_H

#include "../game_obj.h"
#include <map>

class ParticleSystem;
class ParticleEmitter;

// одна частица
struct Particle
{
	v3 position;
	v3 velocity;
	float size;
};

// множество частиц
struct ParticleSet
{
	float countScale;
	bool changed;

	steel::svector<Particle> particles;

	friend class ParticleSystem;
};

class ParticleProcessor
{
protected:
	Config *conf;
	ParticleSet *set;
public:
};

// класс для создания новых частиц
class ParticleEmitter: public ParticleProcessor
{
protected:
	ObjectPosition positionEmitter;
public:
	virtual bool init(Config *_conf, ParticleSet *_set, ObjectPosition _positionEmitter);
	virtual void initParticles();
	virtual void born(Particle &particle) = 0; // создать частицу
	virtual void kill(int i); // убить частицу с номером i
};

// класс для рисования: множество спрайтов, набор объектов, меташарики
class ParticleRenderer: public GraphInterface, public ParticleProcessor
{
protected:
	ResCollection	*res;
	GraphInterface *particleSystem;

public:
	virtual bool init(Config *_conf, ParticleSet *_set, ResCollection	*_res, GraphInterface *_particleSystem);
	virtual bool initParticles() = 0;
};

// класс, который анимирует частицы
class ParticleAnimator: public PhysicInterface, public ParticleProcessor
{
protected:
	ResCollection	*res;
	PhysicInterface *particleSystem;
public:
	virtual bool init(Config *_conf, ParticleSet *_set, ResCollection	*_res, PhysicInterface *_particleSystem);
	virtual bool initParticles() = 0;
};

// система чатиц
class ParticleSystem: public GameObj
{
	Config *conf, *emitterConf, *rendererConf, *animatorConf;
	ParticleSet			 particleSet;
	ParticleEmitter		*emitter;
	ParticleRenderer	*renderer;
	ParticleAnimator	*animator;

public:
	bool init(ScriptLine	&s, ResCollection &res);
	GraphInterfaceList getChildrens()
	{
		GraphInterfaceList a;
		if(renderer) a.push_back(renderer);
		return a;
	}
	PhysicInterfaceList getPChildrens()
	{
		PhysicInterfaceList a;
		if(animator) a.push_back(animator);
		return a;
	}
	PositionKind::PositionKind getPositionKind(void) { return PositionKind::global;}
};

#endif
