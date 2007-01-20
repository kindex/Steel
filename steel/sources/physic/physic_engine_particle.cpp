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
#include "physic_particle.h"


v3 PhysicEngine::calculateForceForParticle(ParticleShadow* shadow1, ParticleShadow* shadow2)
{
	if (shadow1 == shadow2)
	{
		return v3(0,0,0);
	}

	v3 res;
	res.loadZero();

// -------------------------------- PS Forces --------------------------------

	v3& pos1 = shadow1->position;
	v3& pos2 = shadow2->position;

/*	if(helperDrawLines) // draw collision
	{
		helper->drawLine(Line(pos1, pos2-pos1), 0.0f,0.0f, color4f(1.0f,0.0f,0.0f,1.0f));
	}
*/
	
	float dist = (pos2-pos1).getLength();
	
	float spring_r0 = shadow1->spring_r0 + shadow2->spring_r0;
	float spring_k = shadow1->spring_k + shadow2->spring_k;

	float gravity_k = shadow1->gravity_k + shadow2->gravity_k;
	float gravity_power = 0.5f*(shadow1->gravity_power + shadow2->gravity_power);
	float gravity_min_dist = 0.5f*(shadow1->gravity_min_dist + shadow2->gravity_min_dist);

	//res += (pos2-pos1).getNormalized() * (dist - spring_r0)*spring_k; // пружина
	res += (pos2-pos1).getNormalized() * (pow((1/dist),2) - pow(1/dist,3)); // lennard-jones

	if (dist>gravity_min_dist && gravity_k != 0)
	{
		res += (pos2-pos1).getNormalized() * pow(dist, gravity_power)*gravity_k; // гравитация
	}

// --------------------------------------------------------------------------
	shadow2->force -= res; // сила действия равна силе противодействия

	return res;
}

v3 PhysicEngine::calculateForceForParticle(ParticleShadow* shadow)
{
	v3 res;
	res.loadZero();
// реагируем только с частицами, номер которых больше этого
// для того, чтобы исключить повторную проверку (сила действия равна силе противодействия)

	for EACH(ShadowPVector,  shadows, it)
	{
		if (static_cast<ParticleShadow*>(*it)->enabled)
		{
			res += calculateForceForParticle(shadow, static_cast<ParticleShadow*>(*it));
		}
	}

	return res;
}


bool PhysicEngine::processParticle(ParticleShadow* shadow)
{

// -------------------
//	shadow->force += g*objectStorage->mass*0;
	shadow->force += calculateForceForParticle(shadow);

// -------------------

	return true;
}

