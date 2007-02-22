/*id*********************************************************
	File: physic/physic_particle.h
	Unit: physic engine
	Part of: Steel engine
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		���������� ������ - Particle system
 ************************************************************/

#include "physic_particle.h"

bool PhysicEngine::setCurrentObject(GameObject* object)
{
	uid id = object->getId();
	currentShadow = static_cast<ParticleShadow*>(getShadow(id));
	if(currentShadow == NULL)
	{
		return false;
	}

	return false;
}

bool PhysicEngine::isSupportingInterface(IN const InterfaceId id)
{
	return (id & (INTERFACE_PARTICLE_PHYSIC)) == id;
}

void PhysicEngine::addChild(GameObject* child)
{
	addChild(*currentShadow, child);
}

void PhysicEngine::addChild(ParticleShadow& shadow, GameObject* child)
{
	uid childUid = child->getId();
	uidVector::const_iterator it = find(shadow.children, childUid);
	if(it != currentShadow->children.end()) return ; // child have been added before
	if(!child->beforeInject(*this)) return; // shild don't want to be added
	if(!makeShadowForObject(child)) return;
	shadow.children.push_back(childUid);
	setCurrentObject(child);
	child->bindEngine(*this);
	currentShadow = &shadow;
}

void PhysicEngine::deleteChild(GameObject* child)
{
	// TODO:
}

void PhysicEngine::clearChildren()
{
	// TODO:
}

void PhysicEngine::setPosition(IN const v3 position)
{
	currentShadow->position = position;
}

v3 PhysicEngine::getPosition()
{
	return currentShadow->position;
}

void PhysicEngine::setVelocity(IN const v3 velocity)
{
	currentShadow->velocity = velocity;
}

v3 PhysicEngine::getVelocity()
{
	return currentShadow->velocity;
}

void PhysicEngine::setConfig(Config& _config)
{
	currentShadow->config			= &_config;
	currentShadow->mass				= currentShadow->config->getf("mass", 1.0f);
	currentShadow->distance_k		= currentShadow->config->getf("distance_k", 1.0f);
	currentShadow->spring_r0		= currentShadow->config->getf("spring_r0");
	currentShadow->spring_k			= currentShadow->config->getf("spring_k");
	currentShadow->gravity_k		= currentShadow->config->getf("gravity_k");
	currentShadow->gravity_power	= currentShadow->config->getf("gravity_power");
	currentShadow->gravity_min_dist	= currentShadow->config->getf("gravity_min_dist");
	currentShadow->friction_k		= currentShadow->config->getf("friction_k");
	currentShadow->friction_power	= currentShadow->config->getf("friction_power");
	currentShadow->lj_power1		= currentShadow->config->getf("lj_power1", -2);
	currentShadow->lj_power2		= currentShadow->config->getf("lj_power2", -3);
	currentShadow->enabled			= true;
}


Shadow* PhysicEngine::getShadowClass(GameObject *object)
{
	return new ParticleShadow(this);
}

bool PhysicEngine::inject(GameObject *object)
{
	if(!object->isSuportingInterface(*this)) return false;

	// ���� ������ �� ����� �����������
	if(!object->beforeInject(*this)) return false;

	// �������� ������
	if(!makeShadowForObject(object)) return false;
	// ������ ���������� ��������
	objects.push_back(object);

	setCurrentObject(object);
	object->bindEngine(*this);

	return true;
}

bool PhysicEngine::remove(GameObject *object)
{
	deleteShadowForChildren(findSid(object->getId()));
	deleteShadowForObject(findSid(object->getId()));
	
	for EACH(pvector<GameObject*>, objects, it)
		if(*it == object)
		{
			objects.erase(it);
			break;
		}
	object->afterRemove(*this);

	return true;
}

void PhysicEngine::prepare(ParticleShadow* shadow, GameObject* parent)
{
	currentShadow = shadow;

	shadow->force.loadZero();
	shadow->object->updateInformation(*this);

	for EACH(uidVector, shadow->children, it)
	{
		prepare(getShadow(*it), shadow->object);
	}
}

bool PhysicEngine::process(IN const TimeInfo& info)
{
	timeInfo = info;

	int size = objects.size();

	for(int i=0; i < size; i++)
	{
		prepare(getShadow(objects[i]), NULL);
	}
	// update position
	
	for EACH(ShadowPVector,  shadows, it)
	{
		if (static_cast<ParticleShadow*>(*it)->enabled)
		{
			processParticle(static_cast<ParticleShadow*>(*it));
	//		->position += v3(0,0,1)*timeInfo.frameLength;
		}
	}

	for EACH(ShadowPVector,  shadows, it)
	{
	if (static_cast<ParticleShadow*>(*it)->enabled)
	{
		ParticleShadow* shadow = static_cast<ParticleShadow*>(*it);

		shadow->velocity += info.frameLength*shadow->force/shadow->mass;
		shadow->force += shadow->velocity.getNormalized() * shadow->velocity.getNormalized();

		v3 frictionForce  = 
			-shadow->velocity.getNormalized() * pow(shadow->velocity.getLength(), shadow->friction_power)*shadow->friction_k;

		v3 newVelocity = shadow->velocity + info.frameLength*frictionForce/shadow->mass;

		if((newVelocity & shadow->velocity) >0) // �_и�>а �'�_���_и�_ �_�� �_�_����' �_аз�_���_�_�_�'�_ �'���>�_
		{
			shadow->velocity = newVelocity;
		}
		else
		{
			shadow->velocity.loadZero();
		}

		shadow->position += shadow->velocity*info.frameLength;
	}
	}

	return true;
}

bool PhysicEngine::clear()
{
	while(!objects.empty())
	{
		remove(objects.back());
	}

	return true;
}

PhysicEngine::PhysicEngine():
	currentShadow(NULL),
	Engine()
{}