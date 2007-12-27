/*id*********************************************************
	Unit: Demo 19
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system testtype
 ************************************************************/

#ifndef _PARTICLE_CALCULATOR_H_
#define _PARTICLE_CALCULATOR_H_

#include <engine/visitor.h>
#include <objects/combiner/graph_object.h>

class ParticleCalculator : public Visitor
{
public:
    ParticleCalculator(GraphObject* graphObject);
    void clear();
    void postvisit(IN OUT Particle* particle);

    int cnt;
    GraphObject* graphObject;
};

#endif
