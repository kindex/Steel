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

#include "particle_calculator.h"
#include <math/line.h>
#include <math/plane.h>
#include <objects/ps/particle_system.h>

ParticleCalculator::ParticleCalculator(GraphObject& graphObject, std::ofstream& output) : 
	graphObject(graphObject),
    output(output)
{}
        
void ParticleCalculator::clear()
{
    cnt = 0;
    volume = 0;
}

static bool _isPointInObject(IN v3 point, IN const GraphObject& object, const ObjectPosition position)
{
    size_t crossCount = 0;
    
    Line lineSegment(point, v3(0.0, 0.0, 1.0));

    const Faces&        faces    = *object.getAllFaces();
    const VertexVector* vertexes = object.getVertexes();

	for EACH_CONST(TriangleVector, faces.triangles, it)
	{
		Plane triangle;
		triangle.base = position * vertexes->at(it->a[0]);
		triangle.a = position * vertexes->at(it->a[1]) - triangle.base;
		triangle.b = position * vertexes->at(it->a[2]) - triangle.base;
		float k;
		if (isCrossTrgLine(triangle, lineSegment, k))
		{
			if (k > 0.0)
			{
				crossCount++;
			}
		}
	}
	return crossCount%2 == 1;
}


void ParticleCalculator::postvisit(IN OUT Particle* particle)
{
    if (_isPointInObject(particle->position, graphObject, ObjectPosition::getIdentity()))
//	if (particle->position.x > -0.5 && < )  
	{
        cnt++;
		output << "volume create radius " << particle->size << " sphere \n";
		output << "volume move \"volume." << cnt << "\" offset " << particle->position.x << 
                  " " << particle->position.y << 
                  " " << particle->position.y << 
                  "\n";

		float particleHalfSize = particle->size;
		float suffix = 4.0f / 3.0f; 
		float particleVol = particleHalfSize*particleHalfSize*particleHalfSize*suffix;
        volume += float(M_PI*particleVol);
    }
}
