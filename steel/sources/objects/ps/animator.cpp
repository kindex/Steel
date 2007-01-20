/*id*********************************************************
	File: objects/ps/animator.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Обрабатывает движение частиц в системе.
************************************************************/
#include "animator.h"

/*
void UniPSanimator::ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime _modificationTime)
{
	int size = set->particles.size();
	int oldsize = children.size();
	if(size != oldsize)
	{
		if(size > oldsize)
		{
			children.resize(size);
			for(int i = oldsize; i < size; i++)
			{
				children[i] = new UniParticle(set->particles[i], conf);
//				debugi(children[i]->getModificationTime());
			}
		}
		if(size < (int)children.size())
		{
			for(int i = size; i < oldsize; i++)
				delete children[i];
			children.resize(size);
		}

		modificationTime = _modificationTime;
		particleSystem->setChildrenChangeTime(_modificationTime);
	}

	for(int i=0; i<size; i++)
		children[i]->setParticle(set->particles[i]);
}
*/

bool SimpleAnimator::initParticles()
{
	return true;
}

bool UniPSanimator::initParticles()
{ 
	return true;
}

bool UniPSanimator::InitFromConfig(Config& _conf)
{
	conf = &_conf;

	return true;
}

bool UniPSanimator::updateInformation(IN OUT Engine& engine)
{

	return false;
}

void UniPSanimator::process(IN const ProcessInfo& info)
{
	for EACH(pvector<UniParticle*>, children, it)
	{
		(*it)->process(info);
	}
}

bool UniPSanimator::beforeInject(IN OUT Engine& _engine)
{
	engine = static_cast<PhysicEngine*>(&_engine);

	for EACH(pvector<UniParticle*>, children, it)
	{
		engine->addChild(*it);
	}

	return true;
}

void UniPSanimator::afterRemove(IN OUT Engine&) 
{
}

void UniPSanimator::onParticleBorn(int index)
{
	UniParticle* newParticle = new UniParticle(set->particles[index], conf);
	children.push_back(newParticle);

	if (engine != NULL)
	{
		engine->addChild(newParticle);
	}
}

bool UniParticle::InitFromConfig(Config&)
{
	return true;
}

bool UniParticle::updateInformation(IN OUT Engine&)
{
	return false;
}

void UniParticle::process(IN const ProcessInfo& info)
{
	if (engine != NULL)
	{
		engine->setCurrentObject(this);
		particle->position = engine->getPosition();
		particle->velocity = engine->getVelocity();
	}
}

bool UniParticle::beforeInject(IN OUT Engine& engine)
{
	return true;
}

void UniParticle::afterRemove(IN OUT Engine&)
{
}

bool UniParticle::isSuportingInterface(IN OUT Engine& engine)
{
	return engine.isSupportingInterface(INTERFACE_PARTICLE_PHYSIC);
}

void UniParticle::bindEngine(IN OUT Engine& _engine)
{
	engine = static_cast<PhysicEngine*>(&_engine);
	engine->setPosition(particle->position);
	engine->setVelocity(particle->velocity);
}

UniPSanimator::UniPSanimator():
	engine(NULL)
{}

UniParticle::UniParticle(Particle *_particle, Config *_conf):
	particle(_particle),
	conf(_conf),
	engine(NULL)
{}
