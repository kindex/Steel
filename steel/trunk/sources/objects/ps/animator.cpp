/*id*********************************************************
	File: objects/ps/animator.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Обрабатывает движение частиц в системе.
************************************************************/
#include "animator.h"

/*bool UniPSanimator::initParticles()
{
	return true;
}

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
