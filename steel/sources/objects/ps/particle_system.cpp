/*id*********************************************************
	File: objects/ps/particle_system.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Основа систем частиц. Базовый класс, которые компонует
		несколько реализаций эмитера, аниматора и рендерера для
		систем частиц.
************************************************************/
#include "../../steel.h"

#include "particle_system.h"
#include "../game_object_factory.h"
#include "../../res/res_main.h"

#include <string>

ParticleProcessor::ParticleProcessor(): 
	conf(NULL), 
	set(NULL), 
	particleSystem(NULL) 
{}

void ParticleProcessor::bindParticleSystem(ParticleSystem *a) 
{
	particleSystem = a;
	set = &particleSystem->particleSet;
}

bool ParticleProcessor::InitFromConfig(Config& _conf) 
{
	conf = &_conf;
	return true; 
}


ParticleSystem::ParticleSystem():
	conf(NULL),
	emitterConf(NULL),
	rendererConf(NULL),
	animatorConf(NULL),
	emitter(NULL),
	renderer(NULL),
	animator(NULL)
{}

bool ParticleSystem::InitFromConfig(Config& _conf)
{
	conf = &_conf;

	if ((emitterConf = conf->find("emitter")) == NULL)
    {
		abort_init("error res ps", "Cannot find emitter config");
    }
	if ((rendererConf = conf->find("renderer")) == NULL)
    {
		abort_init("error res ps", "Cannot find renderer config");
    }
	if ((animatorConf = conf->find("animator")) == NULL)
    {
		abort_init("error res ps", "Cannot find animator config");
    }

	particleSet.countScale = conf->getf("countScale", 1.0f);

#define INIT_PARTICLE_PROCESSOR(processor, CLASS) \
	processor = gameObjectFactory->create##CLASS(processor##Conf->gets("class"));\
	if(processor == NULL) return false;\
	processor->bindParticleSystem(this);\
	if(!processor->InitFromConfig(*processor##Conf)) \
	{ \
		abort_init("error res ps emitter", "emitter class cannot initialize"); \
	}
	
	INIT_PARTICLE_PROCESSOR(emitter, ParticleEmitter);
	INIT_PARTICLE_PROCESSOR(animator, ParticleAnimator);
	INIT_PARTICLE_PROCESSOR(renderer, ParticleRenderer);

	emitter->initParticles();
	animator->initParticles();
	renderer->initParticles();

	return true;
}

Config* ParticleSystem::getConfig() const
{
    return NULL; // TODO:
}

void ParticleEmitter::kill(int i) // убить частицу с номером i
{

}

bool ParticleEmitter::initParticles()
{
	int initSize = (int)ceil(conf->geti("init_size") * set->countScale);
	set->particles.resize(initSize);
	for (int i=0; i<initSize; i++)
	{
		set->particles[i] = new Particle;
		born(*set->particles[i], i);
	}
	return true;
}

bool ParticleSystem::supportsInterface(Engine& engine, IN const InterfaceId id)
{
	return id == INTERFACE_GRAPH;
}

bool ParticleSystem::updateInformation(Engine& engine, IN const InterfaceId id)
{
	if (id == INTERFACE_GRAPH)
	{
//		emitter->updateInformation(engine);
		renderer->updateInformation(engine, id);
		return true;
	}

	return false;
}

bool ParticleSystem::beforeInject(Engine& engine, IN const InterfaceId id)
{
	if (id == INTERFACE_GRAPH)
	{
		return renderer->beforeInject(engine, id);
	}

	return false;
}

void ParticleSystem::afterRemove(Engine& engine, IN const InterfaceId id)
{
	if (id == INTERFACE_GRAPH)
	{
		return renderer->afterRemove(engine, id);
	}
}

void ParticleSystem::bindEngine(Engine& engine, IN const InterfaceId id)
{
	if (id == INTERFACE_GRAPH)
	{
		return renderer->afterRemove(engine, id);
	}
}

void ParticleSystem::process(IN const ProcessInfo& info)
{
	emitter->process(info);
	animator->process(info);
	renderer->process(info);
}

void ParticleSystem::particleBorn(int index)
{
	emitter->onParticleBorn(index);
	animator->onParticleBorn(index);
	renderer->onParticleBorn(index);
}

void ParticleSystem::particleDie(int index)
{
	emitter->onParticleDie(index);
	animator->onParticleDie(index);
	renderer->onParticleDie(index);
}

void ParticleSystem::traverse(Visitor& visitor, const ObjectPosition& base_position)
{
    if (visitor.visit(this))
    {
        for EACH(ParticleVector, particleSet.particles, it)
        {
            visitor.postvisit(*it);
        }
        visitor.postvisit(this, base_position);
    }
}
