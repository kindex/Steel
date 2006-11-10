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

bool ParticleSystem::init(ScriptLine	&s)
{
	if(!GameObj::init(s)) return false;

	conf = resOldConfig.add(s.gets(3), false); // Dont pop resStack
	if(!conf) return false;

	if(!(emitterConf = resOldConfig.add(conf->gets("emitter")))) 
	{
		resOldConfig.pop();
		abort_init("error res ps", "Cannot find class emitterConf");
	}
	if(!(rendererConf = resOldConfig.add(conf->gets("renderer"))))
	{
		resOldConfig.pop();
		abort_init("error res ps", "Cannot find class rendererConf");
	}
	if(!(animatorConf = resOldConfig.add(conf->gets("animator")))) 
	{
		resOldConfig.pop();
		abort_init("error res ps", "Cannot find class animatorConf");
	}

	particleSet.countScale = conf->getf("countScale", 1);

	std::string emitterClass = emitterConf->gets("class");	emitter = NULL;
	if(emitterClass == "SimpleEmitter")	emitter = new SimpleEmitter;
	if(!emitter) abort_init("error res ps emitter", "Emitter class " + emitterClass + " not found");
	if(!emitter->init(emitterConf, &particleSet, this)) { resOldConfig.pop(); abort_init("error res ps emitter", "Emitter class " + emitterClass + " cannot initialize"); }

	std::string animatorClass = animatorConf->gets("class");	animator = NULL;
	if(animatorClass == "UniPSanimator")	animator = new UniPSanimator;
	if(!animator) abort_init("error res ps renderer", "Animator class " + animatorClass + " not found");
	if(!animator->init(animatorConf, &particleSet, this)) { resOldConfig.pop(); abort_init("error res ps animator", "Animator class " + animatorClass + " cannot initialize"); }

	std::string rendererClass = rendererConf->gets("class");	renderer = NULL;
	
	if(rendererClass == "SpriteRenderer")	renderer = new SpriteRenderer;
	if(rendererClass == "ObjectPSRenderer")	renderer = new ObjectPSRenderer;
	if(rendererClass == "DummyPSRenderer")	renderer = new DummyPSRenderer;

	if(!renderer) abort_init("error res ps renderer", "Renderer class " + rendererClass + " not found");
	if(!renderer->init(rendererConf, &particleSet,  this)) { resOldConfig.pop(); abort_init("error res ps renderer", "Renderer class " + rendererClass + " cannot initialize"); }

	resOldConfig.pop();
	return true;
}

void ParticleEmitter::kill(int i) // убить частицу с номером i
{

}

bool ParticleEmitter::init(OldConfig *_conf, ParticleSet *_set, ParticleSystem *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;	particleSystem = _particleSystem;

	initParticles();

	return true;
}

void ParticleEmitter::initParticles()
{
	int initSize = (int)ceil(conf->geti("init_size") * set->countScale);
	set->modificationTime = -1;
	set->particles.resize(initSize);
	for(int i=0; i<initSize; i++)
	{
		set->particles[i] = new Particle;
		born(*set->particles[i]);
	}
	particleSystem->setChildrenChangeTime(globalFrameNumber);
}

bool ParticleRenderer::init(OldConfig *_conf, ParticleSet *_set, GraphObject *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;
	particleSystem = _particleSystem;

	if(!initParticles()) return false;

	return true;
}

bool ParticleAnimator::init(OldConfig *_conf, ParticleSet *_set, ParticleSystem  *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;
	particleSystem = _particleSystem;

	if(!initParticles()) return false;

	return true;
}