#include "../../_cpp.h"

#include "particle_system.h"
#include "emitter.h"
#include "ps_renderer.h"
#include "animator.h"
#include "../../res/res_main.h"

#include <string>

bool ParticleSystem::init(ScriptLine	&s)
{
	if(!GameObj::init(s)) return false;

	conf = resConfig.add(s.gets(3), false); // Dont pop resStack
	if(!conf) return false;

	if(!(emitterConf = resConfig.add(conf->gets("emitter")))) 
	{
		resConfig.pop();
		abort_init("error res ps", "Cannot find class emitterConf");
	}
	if(!(rendererConf = resConfig.add(conf->gets("renderer"))))
	{
		resConfig.pop();
		abort_init("error res ps", "Cannot find class rendererConf");
	}
	if(!(animatorConf = resConfig.add(conf->gets("animator")))) 
	{
		resConfig.pop();
		abort_init("error res ps", "Cannot find class animatorConf");
	}

	particleSet.countScale = conf->getf("countScale", 1);

	std::string emitterClass = emitterConf->gets("class");	emitter = NULL;
	if(emitterClass == "SimpleEmitter")	emitter = new SimpleEmitter;
	if(!emitter) abort_init("error res ps emitter", "Emitter class " + emitterClass + " not found");
	if(!emitter->init(emitterConf, &particleSet, this)) { resConfig.pop(); abort_init("error res ps emitter", "Emitter class " + emitterClass + " cannot initialize"); }

	std::string animatorClass = animatorConf->gets("class");	animator = NULL;
	if(animatorClass == "UniPSanimator")	animator = new UniPSanimator;
	if(!animator) abort_init("error res ps renderer", "Animator class " + animatorClass + " not found");
	if(!animator->init(animatorConf, &particleSet, this)) { resConfig.pop(); abort_init("error res ps animator", "Animator class " + animatorClass + " cannot initialize"); }

	std::string rendererClass = rendererConf->gets("class");	renderer = NULL;
	if(rendererClass == "SpriteRenderer")	renderer = new SpriteRenderer;
	if(rendererClass == "ObjectPSRenderer")	renderer = new ObjectPSRenderer;
	if(!renderer) abort_init("error res ps renderer", "Renderer class " + rendererClass + " not found");
	if(!renderer->init(rendererConf, &particleSet,  this)) { resConfig.pop(); abort_init("error res ps renderer", "Renderer class " + rendererClass + " cannot initialize"); }

	graphList.push_back(renderer);
	physicList.push_back(animator);

	resConfig.pop();
	return true;
}

void ParticleEmitter::kill(int i) // убить частицу с номером i
{

}

bool ParticleEmitter::init(Config *_conf, ParticleSet *_set, ParticleSystem *_particleSystem)
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
		born(set->particles[i]);
}

bool ParticleRenderer::init(Config *_conf, ParticleSet *_set, GraphInterface *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;
	particleSystem = _particleSystem;

	if(!initParticles()) return false;

	return true;
}

bool ParticleAnimator::init(Config *_conf, ParticleSet *_set, ParticleSystem  *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;
	particleSystem = _particleSystem;

	if(!initParticles()) return false;

	return true;
}
