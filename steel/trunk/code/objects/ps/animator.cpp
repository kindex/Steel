#include "animator.h"

bool UniPSanimator::initParticles()
{
	return true;
}

void UniPSanimator::process(steel::time curTime, steel::time frameLength, ModificationTime _modificationTime)
{
	int size = set->particles.size();
	int oldsize = children.size();
	if(size != oldsize)
	{
		if(size > oldsize)
		{
			children.resize(size);
			for(int i = oldsize; i < size; i++)
				children[i] = new UniParticle(set->particles[i], conf);
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
