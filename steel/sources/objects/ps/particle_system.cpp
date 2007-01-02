/*id*********************************************************
	File: objects/ps/particle_system.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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
//	if(_class == "UniPSanimator") return new UniPSanimator;
	if(_class == "simple") return new SimpleAnimator;

	error("ps", string("ParticleAnimator class '") + _class + "' not found");
	return NULL;
}

ParticleRenderer* findParticleRenderer(const string &_class)
{
	if(_class == "sprite") return new SpriteRenderer;
	if(_class == "object") return new ObjectPSRenderer;
	if(_class == "dummy") return new DummyPSRenderer;

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

bool ParticleProcessor::InitFromConfig(Config *_conf) 
{
	conf = _conf;
	return conf != NULL; 
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

bool ParticleSystem::InitFromConfig(Config *_conf)
{
	if(_conf == NULL) return false;
	conf = _conf;

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
	if(!processor->InitFromConfig(processor##Conf)) \
	{ \
		abort_init("error res ps emitter", "emitter class cannot initialize"); \
	}\
	processor->initParticles();
	
	INIT_PARTICLE_PROCESSOR(emitter, ParticleEmitter);
	INIT_PARTICLE_PROCESSOR(animator, ParticleAnimator);
	INIT_PARTICLE_PROCESSOR(renderer, ParticleRenderer);

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
		born(*set->particles[i]);
	}
	return true;
}

bool ParticleSystem::isSuportingInterface(InterfaceId id)
{
	return id == GraphInterface::interfaceId;
}

bool ParticleSystem::updateInformation(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId)
	{
		emitter->updateInformation(id, engine);
		animator->updateInformation(id, engine);
		renderer->updateInformation(id, engine);
		return true;
	}
//	if(id == PhysicInterface::interfaceId) result ||= animator->updateInformation();
	return false;
}

bool ParticleSystem::beforeInject(InterfaceId id)
{
	if(id == GraphInterface::interfaceId) return renderer->beforeInject(id);
	return false;
}

void ParticleSystem::afterRemove(InterfaceId, Engine* engine)
{
	if (id == GraphInterface::interfaceId) renderer->afterRemove(id, engine);
}

void ParticleSystem::process(IN const ProcessInfo& info)
{
	emitter->process(info);
	animator->process(info);
	renderer->process(info);
}

void ParticleSystem::bindEngine(InterfaceId id, Engine* engine)
{
	if(id == GraphInterface::interfaceId) renderer->bindEngine(id, engine);
}
