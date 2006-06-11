#include "complex_particle_system.h"
#include "../../common/utils.h"

bool Particle::init(v3 emitterPosition, v3 emitterDirection, Config *_conf, ResCollection &_res)
{
	conf = _conf;
	res = &_res;
	if(!conf) return false;
	m = (Material*)res->add(Res::material, conf->gets("material"));

//	conf->getf("aaa") example: reads from config file Veriable named aaa, with float type

	align = SpriteAlign::screen;

	sprite.resize(1);
	sprite[0].pos = v3(0,0,0);
	sprite[0].size = conf->getf("size");
	position.loadIdentity();

	float  position_dispersion = conf->getf("position_dispersion");

	position.setTranslation(v3(frand()*position_dispersion, frand()*position_dispersion, frand()*position_dispersion) + emitterPosition	);

	vel.translation = emitterDirection.getNormalized()*conf->getf("ps_speed");

//	vel.translation.set(frand()*0.1f, frand()*0.1f, frand()*0.1f);

	initSprites();
	return true;
}

bool ComplexParticleSystem::init(ScriptLine	&s, ResCollection &res)
{
	if(!GameObj::init(s, res)) return false;
	
	if(s.count()<3) return false;
	conf = (Config*)res.add(Res::config, s.gets(3));
	if(!conf)
	{
		alog.msg("error res model", "Model config not found: " + s.gets(3));
		return false;
	}
	int count = conf->geti("count");
	particles.resize(count);
	for(int i=0; i<count; i++)
	{
		particles[i] = new Particle;
		particles[i]->init(position.getTranslation(), v3(0,0,0), conf, res);
	}

	return true;
}

GraphInterfaceList ComplexParticleSystem::getChildrens()
{
	GraphInterfaceList a;
	for(steel::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		a.push_back(*it);
	return a;
}
PhysicInterfaceList ComplexParticleSystem::getPChildrens()
{
	PhysicInterfaceList a;
	for(steel::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		a.push_back(*it);
	return a;
}
