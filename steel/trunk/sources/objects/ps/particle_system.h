/*id*********************************************************
	File: objects/ps/particle_system.h
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
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
	ModificationTime modificationTime;

	steel::svector<Particle*> particles;

	friend class ParticleSystem;
};

class ParticleProcessor
{
protected:
	OldConfig *conf;
	ParticleSet *set;
public:
	virtual ~ParticleProcessor(void) {}
};

// класс для создания новых частиц
class ParticleEmitter: public ParticleProcessor
{
protected:
	ParticleSystem *particleSystem;
public:
	virtual bool init(OldConfig *_conf, ParticleSet *_set, ParticleSystem *_particleSystem);
	virtual void initParticles(void);
	virtual void born(Particle &particle) = 0; // создать частицу
	virtual void kill(int i); // убить частицу с номером i
	virtual void ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime) = 0;
};

// класс для рисования: множество спрайтов, набор объектов, меташарики
class ParticleRenderer: public GraphObject, public ParticleProcessor
{
protected:
	GraphObject *particleSystem;

public:
	virtual bool init(OldConfig *_conf, ParticleSet *_set, GraphObject *_particleSystem);
	virtual bool initParticles(void) = 0;
};

// класс, который анимирует частицы
class ParticleAnimator: public PhysicObject, public ParticleProcessor
{
protected:
	ParticleSystem *particleSystem;
public:
	virtual bool init(OldConfig *_conf, ParticleSet *_set, ParticleSystem *_particleSystem);
	virtual bool initParticles(void) = 0;
};

// система чатиц
class ParticleSystem: public GameObj
{
	OldConfig *conf, *emitterConf, *rendererConf, *animatorConf;
	ParticleSet			 particleSet;
	ParticleEmitter		*emitter;
	ParticleRenderer	*renderer;
	ParticleAnimator	*animator;

public:
	bool init(ScriptLine	&s);

	int getPhysicChildrenCount(void) { return animator?1:0; }
	PhysicObject* getPhysicChildren(int i) { return animator; }

	int getGraphChildrenCount(void) { return renderer?1:0; }
	GraphObject* getGraphChildren(int i) { return renderer; }


	PositionKind getPositionKind(void) { return POSITION_GLOBAL;}

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
