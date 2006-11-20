/*id*********************************************************
	File: objects/ps/particle_system.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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

	steel::svector<Particle*> particles;
};

class ParticleSystem;

class ParticleProcessor: public GameObject
{
public:
	ParticleProcessor(void);
	virtual ~ParticleProcessor(void) {}
	virtual void bindParticleSystem(ParticleSystem *a);
	virtual bool initParticles(void) = 0;
	bool InitFromConfig(Config *_conf);

protected:
	Config *conf;
	ParticleSet *set;
	ParticleSystem *particleSystem;
};

// класс для создания новых частиц
class ParticleEmitter: public ParticleProcessor
{
public:
	bool initParticles(void);
	virtual void born(Particle &particle) = 0; // создать частицу
	virtual void kill(int i); // убить частицу с номером i

protected:
	v3 position;
};

// класс для рисования: множество спрайтов, набор объектов, меташарики
class ParticleRenderer: public ParticleProcessor
{
};

// класс, который анимирует частицы (физика частиц)
class ParticleAnimator: public ParticleProcessor
{
};

// система чатиц
class ParticleSystem: public GameObject
{
	Config				*conf;
	Config				*emitterConf;
	Config				*rendererConf;
	Config				*animatorConf;
	ParticleEmitter		*emitter;
	ParticleRenderer	*renderer;
	ParticleAnimator	*animator;
	ParticleSet			 particleSet;

friend class ParticleProcessor;

public:
	ParticleSystem(void);

	bool InitFromConfig(Config *conf);
	bool isSuportingInterface(InterfaceId);
	bool updateInformation(InterfaceId, Engine*);
	bool beforeInject(InterfaceId);
	void afterRemove(InterfaceId);
	void process(ProcessInfo &);
	void bindEngine(InterfaceId, Engine*);
};

#endif
