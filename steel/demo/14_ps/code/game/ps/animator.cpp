#include "animator.h"

bool UniPSanimator::initParticles()
{
	int size = set->particles.size();
	children.resize(size);
	for(int i = 0; i < size; i++)
		children[i] = new UniParticle(&set->particles[i], conf);


	return true;
}
