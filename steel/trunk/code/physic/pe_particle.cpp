/*id*********************************************************
	File: physic/pe_particle.cpp
	Unit: steel physic engine/particles
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Steel Версия физического движка. 
		Обрабатывает движение только частиц.
************************************************************/

#include "../steel.h"
#include "physic_engine_steel.h"

v3 PhysicEngineSteel::calculateForceForParticle(PhysicStorageParticle *storage1, PhysicStorageParticle *storage2)
{
	v3 res;
	res.loadZero();

// -------------------------------- PS Forces --------------------------------

	v3 &pos1 = storage1->position;
	v3 &pos2 = storage2->position;

	if(helperDrawLines) // draw collision
		helper->drawLine(Line(pos1, pos2-pos1), 0.0f,0.0f, color4f(1.0f,0.0f,0.0f,1.0f));

	Config *material1 =  ((PhysicObject*)storage1->object)->getPMaterial();
	Config *material2 =  ((PhysicObject*)storage2->object)->getPMaterial();

	float dist = (pos2-pos1).getLength();
	
	float spring_r0 = storage1->spring_r0 + storage2->spring_r0;
	float spring_k = storage1->spring_k + storage2->spring_k;

	float gravity_k = storage1->gravity_k + storage2->gravity_k;
	float gravity_power = 0.5f*(storage1->gravity_power + storage2->gravity_power);
	float gravity_min_dist = 0.5f*(storage1->gravity_min_dist + storage2->gravity_min_dist);

	//res += (pos2-pos1).getNormalized() * (dist - spring_r0)*spring_k; // пружина
	res += (pos2-pos1).getNormalized() * (pow((1/dist),2) - pow(1/dist,3)); // lennard-jones

	if(dist>gravity_min_dist && gravity_k != 0)
	{
		res += (pos2-pos1).getNormalized() * pow(dist, gravity_power)*gravity_k; // гравитация
	}

// --------------------------------------------------------------------------
	storage2->force -= res; // сила действия равна силе противодействия

	return res;
}

v3 PhysicEngineSteel::calculateForceForParticle(PhysicStorageParticle *storage1)
{
	v3 res;
	res.loadZero();
// реагируем только с частицами, номер которых больше этого
// для того, чтобы исключить повторную проверку (сила действия равна силе противодействия)
	for(unsigned int i = storage1->partiecleSetId + 1; i < particleSet.size(); i++)
		res += calculateForceForParticle(storage1, (PhysicStorageParticle*)storages[particleSet[i]]);

	return res;
}


bool PhysicEngineSteel::processParticle(PhysicStorageParticle *objectStorage, steel::time globalTime, steel::time time)
{

// -------------------
	objectStorage->force += g*objectStorage->mass*0;
	objectStorage->force += calculateForceForParticle(objectStorage);

// -------------------

	return true;
}

bool PhysicEngineSteel::PhysicStorageParticle::cache()
{
//		if(collisionType == COLLISION_PARTICLE)
//	debugi(((PhysicObject*)object)->getModificationTime());

	if(PhysicStorage::cache())
	{
		spring_r0			= material->getf("spring_r0");
		spring_k			= material->getf("spring_k");
		gravity_k			= material->getf("gravity_k");
		gravity_power		= material->getf("gravity_power");
		gravity_min_dist	= material->getf("gravity_min_dist");
		friction_k			= material->getf("friction_k");
		friction_power		= material->getf("friction_power");
		return true;
	}
	else
		return false;
}
