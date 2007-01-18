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

// множество частиц
struct ParticleSet
{
	float countScale;

	pvector<Particle*> particles;
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

protected:
	Config *conf;
	ParticleSet *set;
	ParticleSystem *particleSystem;
};

// класс для создания новых частиц
class ParticleEmitter: public ParticleProcessor
{
public:
	bool initParticles();
	virtual void born(Particle &particle) abstract; // создать частицу
	virtual void kill(int i); // убить частицу с номером i

protected:
	v3 position;
};

// класс для рисования: множество спрайтов, набор объектов, меташарики
class ParticleRenderer: public ParticleProcessor
{
public:
	virtual bool updateInformation(IN OUT Engine&) abstract;
	virtual bool beforeInject(IN OUT Engine&) abstract;
	virtual void afterRemove(IN OUT Engine&) abstract;
};

// класс, который анимирует частицы (физика частиц)
class ParticleAnimator: public ParticleProcessor
{
public:
	virtual bool updateInformation(IN OUT Engine&) abstract;
};

// система чатиц
class ParticleSystem: public GameObject
{
	friend class ParticleProcessor;

public:
	ParticleSystem();

	bool isSuportingInterface(IN OUT Engine&);
	bool beforeInject(IN OUT Engine&);
	void afterRemove(IN OUT Engine&);
	bool updateInformation(IN OUT Engine&);
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);

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
