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
		Физический джижок - Particle system
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

void PhysicEngine::setConfig(Config&)
{

}


Engine::Shadow* PhysicEngine::getShadowClass(GameObject *object)
{
	return new ParticleShadow(this);
}

bool PhysicEngine::inject(GameObject *object)
{
	if(!object->isSuportingInterface(*this)) return false;

	// если объект не хочет добавляться
	if(!object->beforeInject(*this)) return false;

	// кешируем объект
	if(!makeShadowForObject(object)) return false;
	// список глобальных объектов
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
//		processParticle(storages[*it]);
		static_cast<ParticleShadow*>(*it)->position += v3(0,0,1)*timeInfo.frameLength;
	}
	

/*	for(svector<int>::iterator it = particleSet.begin(); it != particleSet.end(); it++)
	{
		PhysicStorageParticle *objectStorage = (PhysicStorageParticle*)storages[*it];

		objectStorage->velocity += time*objectStorage->force/objectStorage->mass;
		objectStorage->force += objectStorage->velocity.getNormalized() * objectStorage->velocity.getNormalized();
		v3 frictionForce  = 
			-objectStorage->velocity.getNormalized() * pow(objectStorage->velocity.getLength(), objectStorage->friction_power)*objectStorage->friction_k;

		v3 newVelocity = objectStorage->velocity + time*frictionForce/objectStorage->mass;

		if((newVelocity & objectStorage->velocity) >0) // С_РёР>Р° С'С_РчР_РёС_ Р_Рч Р_Р_РРчС' С_Р°Р·Р_РчС_Р_С_С'С_ С'РчР>Р_
				objectStorage->velocity = newVelocity;
		else
			objectStorage->velocity.loadZero();


		objectStorage->position += objectStorage->velocity*time;

		PhysicObject &object = *(PhysicObject*)objectStorage->object;

		ObjectPosition objectPosition = object.getPosition();
		objectPosition.setTranslation(objectStorage->position);
		object.setPosition(objectPosition);

		velocity vel;
		vel.translation = objectStorage->velocity;
		object.setVelocity(vel);
	}
*/

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
