#include "../../_cpp.h"

#include "particle_system.h"
#include "emitter.h"
#include "ps_renderer.h"
#include "animator.h"

#include <string>

bool ParticleSystem::init(ScriptLine	&s, ResCollection &res)
{
	if(!GameObj::init(s, res)) return false;

	conf = (Config*)res.add(Res::config, s.gets(3));
	if(!conf) return false;

	if(!(emitterConf = (Config*)res.add(Res::config, conf->gets("emitter")))) abort_init("error res ps", "Cannot find class emitterConf");
	if(!(rendererConf = (Config*)res.add(Res::config, conf->gets("renderer")))) abort_init("error res ps", "Cannot find class rendererConf");
	if(!(animatorConf = (Config*)res.add(Res::config, conf->gets("animator")))) abort_init("error res ps", "Cannot find class animatorConf");

	particleSet.countScale = conf->getf("countScale", 1);

	std::string emitterClass = emitterConf->gets("class");	emitter = NULL;
	if(emitterClass == "SimpleEmitter")	emitter = new SimpleEmitter;
	if(!emitter) abort_init("error res ps emitter", "Emitter class " + emitterClass + " not found");
	if(!emitter->init(emitterConf, &particleSet, getPosition())) abort_init("error res ps emitter", "Emitter class " + emitterClass + " cannot initialize");

	std::string animatorClass = animatorConf->gets("class");	animator = NULL;
	if(animatorClass == "UniPSanimator")	animator = new UniPSanimator;
	if(!animator) abort_init("error res ps renderer", "Animator class " + animatorClass + " not found");
	if(!animator->init(animatorConf, &particleSet, &res, this)) abort_init("error res ps animator", "Animator class " + animatorClass + " cannot initialize");


	std::string rendererClass = rendererConf->gets("class");	renderer = NULL;
	if(rendererClass == "SpriteRenderer")	renderer = new SpriteRenderer;
	if(!renderer) abort_init("error res ps renderer", "Renderer class " + rendererClass + " not found");
	if(!renderer->init(rendererConf, &particleSet, &res, this)) abort_init("error res ps renderer", "Renderer class " + rendererClass + " cannot initialize");

	return true;
}

void ParticleEmitter::kill(int i) // убить частицу с номером i
{

}

bool ParticleEmitter::init(Config *_conf, ParticleSet *_set, ObjectPosition _positionEmitter)
{
	if(!(conf = _conf)) return false;
	set = _set;	positionEmitter = _positionEmitter;

	initParticles();

	return true;
}

void ParticleEmitter::initParticles()
{
	int initSize = (int)ceil(conf->geti("init_size") * set->countScale);
	set->changed = true;
	set->particles.resize(initSize);
	for(int i=0; i<initSize; i++)
		born(set->particles[i]);
}

bool ParticleRenderer::init(Config *_conf, ParticleSet *_set, ResCollection	*_res, GraphInterface *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;	res = _res;
	particleSystem = _particleSystem;

	if(!initParticles()) return false;

	return true;
}

bool ParticleAnimator::init(Config *_conf, ParticleSet *_set, ResCollection	*_res, PhysicInterface  *_particleSystem)
{
	if(!(conf = _conf)) return false;
	set = _set;	res = _res;
	particleSystem = _particleSystem;

	if(!initParticles()) return false;

	return true;
}
