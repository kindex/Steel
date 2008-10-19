/*id*********************************************************
	Unit: 23 Ageia tech
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system using Ageia
 ************************************************************/

#include "ageia_ps.h"
#include "23_ageia_tech.h"
#include <nxscene.h>
#include <nxactor.h>
#include <NxSphereShapeDesc.h>

bool AgeiaPsAnimator::initParticles()
{ 
	return true;
}

bool AgeiaPsAnimator::InitFromConfig(Config& _conf)
{
	conf = &_conf;

	particleConf = conf->find("particle");

	return particleConf != NULL;
}

bool AgeiaPsAnimator::updateInformation(IN OUT Engine& engine, IN const InterfaceId id)
{
    return false;
}

void AgeiaPsAnimator::process(IN const ProcessInfo& info)
{
	// Render all actors
	int nbActors = globalScene->getNbActors();
	NxActor** actors = globalScene->getActors();
	while (nbActors--)
	{
		NxActor* actor = *actors++;
		if (!actor->userData)
		{
			continue;
		}

		int index = ((int)actor->userData)-1;

		float glMat[16];
		actor->getGlobalPose().getColumnMajor44(glMat);
		v3 zero;
		zero.loadZero();
		matrix34 m;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m.data.matrix.data.a[i * 3 + j] = glMat[i * 4 + j];
			}
		}

		set->particles[index]->position = v3(glMat[3 * 4 + 0], glMat[3 * 4 + 1], glMat[3 * 4 + 2]);
	}
}

bool AgeiaPsAnimator::beforeInject(IN OUT Engine& _engine, IN const InterfaceId id)
{
	return true;
}

void AgeiaPsAnimator::afterRemove(IN OUT Engine&, IN const InterfaceId id) 
{
}

void AgeiaPsAnimator::onParticleBorn(int index)
{
	if (globalScene == NULL) 
    {
        return;
    }

	// Create body
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping	= 0.5f;

	v3 pos = set->particles[index]->position;
	float size = set->particles[index]->size;
	NxSphereShapeDesc boxDesc;
	boxDesc.radius = size;

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t.x  = pos.x;
	actorDesc.globalPose.t.y  = pos.y;
	actorDesc.globalPose.t.z  = pos.z;
	globalScene->createActor(actorDesc)->userData = (void*)(index+1);
}


AgeiaPsAnimator::AgeiaPsAnimator():
	particleConf(NULL)
{}
