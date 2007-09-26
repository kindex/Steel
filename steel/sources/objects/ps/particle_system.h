/*id*********************************************************
	File: objects/ps/particle_system.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	Description:
		Основа систем частиц. Базовый класс, которые компонует
		несколько реализаций эмитера, аниматора и рендерера для
		систем частиц.
************************************************************/

#ifndef __PARTICLE_SYSTEM_H
#define __PARTICLE_SYSTEM_H

#include "../../engine/game_object.h"

class ParticleSystem;
class ParticleEmitter;

// одна частица
struct Particle
{
	v3 position;
	v3 velocity;
	// direction - angle
	float size;
};

typedef pvector<Particle*> ParticleVector;
// множество частиц
struct ParticleSet
{
	float countScale;

	ParticleVector particles;
};

class ParticleSystem;

class ParticleProcessor
{
public:
	ParticleProcessor();
	virtual ~ParticleProcessor() {}
	virtual void bindParticleSystem(ParticleSystem *a);
	virtual bool InitFromConfig(Config&) abstract;
	virtual bool initParticles() abstract;
	virtual void process(IN const ProcessInfo& info) abstract;
	virtual void onParticleBorn(int index) abstract;
	virtual void onParticleDie(int index) abstract;

protected:
	Config*			conf;
	ParticleSet*	set;
	ParticleSystem*	particleSystem;
};

// класс для создания новых частиц
class ParticleEmitter: public ParticleProcessor
{
public:
	bool initParticles();
	virtual void born(Particle &particle, int index) abstract; // создать частицу
	virtual void kill(int i); // убить частицу с номером i

protected:
	v3 position;
};

// класс для рисования: множество спрайтов, набор объектов, меташарики
class ParticleRenderer: public ParticleProcessor
{
public:
	virtual bool updateInformation(IN OUT Engine&, IN const InterfaceId id) abstract;
    virtual bool beforeInject(IN OUT Engine&, IN const InterfaceId id) { return true; }
    virtual void afterRemove(IN OUT Engine&, IN const InterfaceId id) {}
};

// класс, который анимирует частицы (физика частиц)
class ParticleAnimator: public ParticleProcessor
{
public:
	virtual bool updateInformation(IN OUT Engine&, IN const InterfaceId id) abstract;
    virtual bool beforeInject(IN OUT Engine&, IN const InterfaceId id) {return true; }
    virtual void afterRemove(IN OUT Engine&, IN const InterfaceId id) {}
};

// система чатиц
class ParticleSystem: public GameObject
{
	friend class ParticleProcessor;

public:
	ParticleSystem();

	bool supportsInterface(IN OUT Engine&, IN const InterfaceId);
	bool beforeInject(IN OUT Engine&, IN const InterfaceId);
	void afterRemove(IN OUT Engine&, IN const InterfaceId);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
	void bindEngine(IN OUT Engine&, IN const InterfaceId);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);
    void traverse(Visitor&);

	void particleBorn(int index);
	void particleDie(int index);

private:
	Config				*conf;
	Config				*emitterConf;
	Config				*rendererConf;
	Config				*animatorConf;
	ParticleEmitter		*emitter;
	ParticleRenderer	*renderer;
	ParticleAnimator	*animator;
	ParticleSet			 particleSet;
};

#endif
