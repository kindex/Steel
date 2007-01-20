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
#include "emitter.h"
#include "ps_renderer.h"
#include "animator.h"
#include "../../res/res_main.h"

#include <string>

ParticleEmitter* findParticleEmitter(const string &_class)
{
	if(_class == "simple") return new SimpleEmitter;

	error("ps", string("ParticleEmitter class '") + _class + "' not found");
	return NULL;
}

ParticleAnimator* findParticleAnimator(const string &_class)
{
	if(_class == "UniPSanimator") return new UniPSanimator;
	if(_class == "simple") return new SimpleAnimator;

	error("ps", string("ParticleAnimator class '") + _class + "' not found");
	return NULL;
}

ParticleRenderer* findParticleRenderer(const string &_class)
{
	if(_class == "sprite") return new SpriteRenderer;
	if(_class == "object") return new ObjectPSRenderer;
	if(_class == "dummy")  return new DummyPSRenderer;

	error("ps", string("ParticleRenderer class '") + _class + "' not found");
	return NULL;
}


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

	if( (emitterConf = conf->find("emitter")) == NULL)
		abort_init("error res ps", "Cannot find emitter config");
	if( (rendererConf = conf->find("renderer")) == NULL)
		abort_init("error res ps", "Cannot find renderer config");
	if( (animatorConf = conf->find("animator")) == NULL)
		abort_init("error res ps", "Cannot find animator config");

	particleSet.countScale = conf->getf("countScale", 1.0f);

#define INIT_PARTICLE_PROCESSOR(processor, CLASS) \
	processor = find##CLASS(processor##Conf->gets("class"));\
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

void ParticleEmitter::kill(int i) // убить частицу с номером i
{

}

bool ParticleEmitter::initParticles()
{
	int initSize = (int)ceil(conf->geti("init_size") * set->countScale);
	set->particles.resize(initSize);
	for(int i=0; i<initSize; i++)
	{
		set->particles[i] = new Particle;
		born(*set->particles[i], i);
	}
	return true;
}

bool ParticleSystem::isSuportingInterface(Engine& engine)
{
	return engine.isSupportingInterface(INTERFACE_GRAPH) ||
			engine.isSupportingInterface(INTERFACE_PARTICLE_PHYSIC);
}

bool ParticleSystem::updateInformation(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
//		emitter->updateInformation(engine);
		renderer->updateInformation(engine);
		return true;
	}
	if (engine.isSupportingInterface(INTERFACE_PARTICLE_PHYSIC))
	{
		animator->updateInformation(engine);
	}

	return false;
}

bool ParticleSystem::beforeInject(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
		return renderer->beforeInject(engine);
	}
	if (engine.isSupportingInterface(INTERFACE_PARTICLE_PHYSIC))
	{
		return animator->beforeInject(engine);
	}
	return false;
}

void ParticleSystem::afterRemove(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
		return renderer->afterRemove(engine);
	}
	if (engine.isSupportingInterface(INTERFACE_PARTICLE_PHYSIC))
	{
		return animator->afterRemove(engine);
	}
}

void ParticleSystem::bindEngine(Engine& engine)
{
	if (engine.isSupportingInterface(INTERFACE_GRAPH))
	{
		return renderer->afterRemove(engine);
	}
	if (engine.isSupportingInterface(INTERFACE_PARTICLE_PHYSIC))
	{
		return animator->afterRemove(engine);
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
