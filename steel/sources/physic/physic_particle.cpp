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
#include "../common/containers/string_dict.h"
#include "../common/logger.h"

bool PhysicEngine::setCurrentObject(GameObject* object, const InterfaceId id)
{
	uid objectId = object->getId();
	
    switch (id)
    {
        case INTERFACE_PARTICLE_PHYSIC:
            currentParticleShadow = getParticleShadow(objectId);
            currentShadow = currentParticleShadow;
            currentPolyhedraShadow = NULL;
            if (currentParticleShadow == NULL)
            {
                return false;
            }
            break;

        case INTERFACE_POPYHEDRA_PHYSIC:
            currentParticleShadow = NULL;
            currentPolyhedraShadow = getPolyhedraShadow(objectId);
            currentShadow = currentPolyhedraShadow;

            if (currentPolyhedraShadow == NULL)
            {
                return false;
            }
            break;

        default:
            return false;
    }

	return false;
}

PhysicEngine::ParticleShadow* PhysicEngine::getParticleShadow(uid id)
{
    int sid = findSid(id, INTERFACE_PARTICLE_PHYSIC);
	if (sid >= 0)
    {
		return particles[sid];
    }
	else
    {
		return NULL;
    }
}

PhysicEngine::PolyhedraShadow* PhysicEngine::getPolyhedraShadow(uid id)
{
    int sid = findSid(id, INTERFACE_POPYHEDRA_PHYSIC);
	if (sid >= 0)
    {
		return polyhedras[sid];
    }
	else
    {
		return NULL;
    }
}


void PhysicEngine::addChild(GameObject* child)
{
	addChild(*currentShadow, child);
}

void PhysicEngine::addChild(Shadow& shadow, GameObject* child)
{
	uid childUid = child->getId();
	uidVector::const_iterator it = find(shadow.children, childUid);
	if (it != currentShadow->children.end())
    {
        return ; // child have been added before
    }
    InterfaceId id;
    if (child->supportsInterface(*this, INTERFACE_PARTICLE_PHYSIC))
    {
        id = INTERFACE_PARTICLE_PHYSIC;
    }
    else if (child->supportsInterface(*this, INTERFACE_POPYHEDRA_PHYSIC))
    {
        id = INTERFACE_POPYHEDRA_PHYSIC;
    }
    else
    {
        return;
    }

    if (!child->beforeInject(*this, id))
    {
        return; // shild don't want to be added
    }
	if (!makeShadowForObject(child, id))
    {
        return;
    }
	shadow.children.push_back(childUid);
	setCurrentObject(child, id);
	child->bindEngine(*this, id);
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
	currentParticleShadow->position = position;
}

v3 PhysicEngine::getPosition()
{
	return currentParticleShadow->position;
}

void PhysicEngine::setVelocity(IN const v3 velocity)
{
	currentParticleShadow->velocity = velocity;
}

v3 PhysicEngine::getVelocity()
{
	return currentParticleShadow->velocity;
}

void PhysicEngine::setConfig(Config& _config)
{
	currentParticleShadow->config			= &_config;
	currentParticleShadow->mass				= currentParticleShadow->config->getf("mass", 1.0f);
	currentParticleShadow->distance_k		= currentParticleShadow->config->getf("distance_k", 1.0f);
	currentParticleShadow->spring_r0		= currentParticleShadow->config->getf("spring_r0");
	currentParticleShadow->spring_k			= currentParticleShadow->config->getf("spring_k");
	currentParticleShadow->gravity_k		= currentParticleShadow->config->getf("gravity_k");
	currentParticleShadow->gravity_power	= currentParticleShadow->config->getf("gravity_power");
	currentParticleShadow->gravity_min_dist	= currentParticleShadow->config->getf("gravity_min_dist");
	currentParticleShadow->friction_k		= currentParticleShadow->config->getf("friction_k");
	currentParticleShadow->friction_power	= currentParticleShadow->config->getf("friction_power");
	currentParticleShadow->lj_power1		= currentParticleShadow->config->getf("lj_power1", -2);
	currentParticleShadow->lj_power2		= currentParticleShadow->config->getf("lj_power2", -3);
	currentParticleShadow->global_gravity_k = currentParticleShadow->config->getf("global_gravity_k", 1.0f);
	currentParticleShadow->force_limit      = currentParticleShadow->config->getf("force_limit", INF);
	currentParticleShadow->max_dist         = currentParticleShadow->config->getf("max_dist", INF);

	currentParticleShadow->enabled			= true;
}

void PhysicEngine::setVertexes(const VertexVector* vertexes)
{
    currentPolyhedraShadow->vertexes = vertexes;
}

void PhysicEngine::setFaces(const Faces* faces)
{
    currentPolyhedraShadow->faces = faces;
}

void PhysicEngine::setPosition(const ObjectPosition& position)
{
    currentPolyhedraShadow->position = position;
}

void PhysicEngine::setPositionKind(PositionKind kind)
{
    currentPolyhedraShadow->positionKind = kind;
}


Shadow* PhysicEngine::shadowClassFactory(GameObject* object, const InterfaceId id)
{
    switch (id)
    {
        case INTERFACE_PARTICLE_PHYSIC:
            return new ParticleShadow(this);

        case INTERFACE_POPYHEDRA_PHYSIC:
            return new PolyhedraShadow(this);

        default:
            assert(false, "");
    }
}

bool PhysicEngine::inject(GameObject* object)
{
    InterfaceId id;
    InterfaceId id2;
    if (object->supportsInterface(*this, INTERFACE_PARTICLE_PHYSIC))
    {
        id  = INTERFACE_PARTICLE_PHYSIC;
        id2 = INTERFACE_POPYHEDRA_PHYSIC;
    }
    else if (object->supportsInterface(*this, INTERFACE_POPYHEDRA_PHYSIC))
    {
        id2 = INTERFACE_PARTICLE_PHYSIC;
        id  = INTERFACE_POPYHEDRA_PHYSIC;
    }
    else
    {
        return false;
    }

	// если объект не хочет добавляться
	if (!object->beforeInject(*this, id))
    {
        return false;
    }
	// кешируем объект
	if (!makeShadowForObject(object, id))
    {
        return false;
    }
	// список глобальных объектов
	objects.push_back(object);

	setCurrentObject(object, id);
    object->bindEngine(*this, id);

    if (object->supportsInterface(*this, id2))
    {
	    if (!object->beforeInject(*this, id2))
        {
            return false;
        }
	    if (!makeShadowForObject(object, id2))
        {
            return false;
        }
	    setCurrentObject(object, id2);

        object->bindEngine(*this, id2);
    }

	return true;

}

int PhysicEngine::addShadow(Shadow* newStorage, const InterfaceId id)
{
    switch (id)
    {
        case INTERFACE_PARTICLE_PHYSIC:
            particles.push_back(static_cast<ParticleShadow*>(newStorage));
            return particles.size() - 1;

        case INTERFACE_POPYHEDRA_PHYSIC:
            polyhedras.push_back(static_cast<PolyhedraShadow*>(newStorage));
            return polyhedras.size() - 1;

        default:
            assert(false, "");
            throw;
    }
}


bool PhysicEngine::remove(GameObject* object)
{
    bool result;
    int id = findSid(object->getId(), INTERFACE_PARTICLE_PHYSIC);
    if (id >= 0)
    {
        result = remove(object, INTERFACE_PARTICLE_PHYSIC);
    }
    id = findSid(object->getId(), INTERFACE_POPYHEDRA_PHYSIC);
    if (id >= 0)
    {
        result = result || remove(object, INTERFACE_POPYHEDRA_PHYSIC);
    }
    return result;
}

bool PhysicEngine::remove(GameObject* object, const InterfaceId id)
{
	deleteShadowForChildren(findSid(object->getId(), id), id);
	deleteShadowForObject(findSid(object->getId(), id), id);
	
	for EACH(pvector<GameObject*>, objects, it)
    {
		if (*it == object)
		{
			objects.erase(it);
			break;
		}
    }
	object->afterRemove(*this, INTERFACE_PARTICLE_PHYSIC);

	return true;
}

void PhysicEngine::prepare(ParticleShadow* shadow, GameObject* parent)
{
	currentShadow = shadow;
	currentParticleShadow = shadow;

	shadow->force.loadZero();
	shadow->object->updateInformation(*this, INTERFACE_PARTICLE_PHYSIC);

	for EACH(uidVector, shadow->children, it)
	{
		prepare(getParticleShadow(*it), shadow->object);
	}
}

void PhysicEngine::updateRealPosition(IN OUT PolyhedraShadow* object)
{
	if (object->realPositionCalculated)
    {
        return;
    }

	if (object->positionKind == POSITION_GLOBAL || object->positionKind == POSITION_SCREEN || object->parent == NULL)
	{
		object->realPosition = object->position;
		object->realPositionCalculated = true;
	}
	else
	{
        PolyhedraShadow* parent = getPolyhedraShadow(object->parent->getId());
		updateRealPosition(parent);
		object->realPosition = object->position * parent->realPosition;
		object->realPositionCalculated = true;
	}
}


bool PhysicEngine::process(IN const TimeInfo& info)
{
	timeInfo = info;
    timeInfo.frameLength *= speedup;
//    timeInfo.currentTime *= speedup; // TODO: precize time with speedup

	for EACH(ParticleShadowVector, particles, particle)
	{
		prepare(*particle, NULL);
	}
    for EACH(PolyhedraShadowVector, polyhedras, polyhedra)
	{
		(*polyhedra)->realPositionCalculated = false;
        setCurrentObject((*polyhedra)->object, INTERFACE_POPYHEDRA_PHYSIC);
        (*polyhedra)->object->updateInformation(*this, INTERFACE_POPYHEDRA_PHYSIC);
	}
    for EACH(PolyhedraShadowVector, polyhedras, polyhedra)
	{
		updateRealPosition(*polyhedra);
        if ((*polyhedra)->faces != NULL)
        {
            const ObjectPosition& realPosition =  (*polyhedra)->realPosition;
            (*polyhedra)->triangles.clear();
            const VertexVector& vertexes = *(*polyhedra)->vertexes;
            VertexVector realVertexes;
            realVertexes.reserve(vertexes.size());
            for EACH_CONST(VertexVector, vertexes, vertexIt)
            {
                realVertexes.push_back(*vertexIt*realPosition);
            }

            for EACH_CONST(TriangleVector, (*polyhedra)->faces->triangles, trgIt)
            {
                const Triangle& trgIndex = *trgIt;
                (*polyhedra)->triangles.push_back(Plane(realVertexes[trgIndex.a[0]], 
                                                        realVertexes[trgIndex.a[1]] - realVertexes[trgIndex.a[0]], 
                                                        realVertexes[trgIndex.a[2]] - realVertexes[trgIndex.a[0]]));
            }
        }
	}

	// update position
	
	for EACH(ParticleShadowVector, particles, it)
	{
		if ((*it)->enabled)
		{
            ParticleShadow& shadow = **it;

        	shadow.force += calculateForceForParticle(shadow);

            for EACH_CONST(PolyhedraShadowVector, polyhedras, polyhedra)
            {
                float mindist = INF;
                v3 minvec;
                for EACH_CONST(PlaneVector, (*polyhedra)->triangles, triangleIt)
                {
                    const Plane& triangle = *triangleIt;

                    v3 dist;
                    dist = getPointTrgVector(shadow.position, triangle);
                    float curdist = dist.getLength();
                    if (mindist > curdist)
                    {
                        mindist = curdist;
                        minvec = dist;
                    }
                    //float k;
                    //if (isCrossTrgLine(triangle, Line(shadow->position, shift), k) && k >= 0 && k <= mink && k <= 1 + EPSILON)
                    //{
                    //    mink = k;
                    //    collisionTriangle = triangle;
                    //    wasCollision = true;
                    //}
                }

                if (mindist < shadow.max_dist)
                {
                    v3 wallforce = calcLennardJones(v3(0,0,0),
                                                    minvec,
                                                    shadow.lj_power1,
                                                    shadow.lj_power2,
                                                    shadow.distance_k);

                    shadow.force += wallforce;
                }
            }
		}
	}

	for EACH(ParticleShadowVector, particles, it)
	{
	    if ((*it)->enabled)
	    {
		    ParticleShadow& shadow = **it;

            float forceScalar = shadow.force.getLength();
            if (forceScalar > shadow.force_limit)
            {
                shadow.force *= shadow.force_limit/forceScalar;
            }

		    shadow.velocity += timeInfo.frameLength*shadow.force/shadow.mass;
//		    shadow->force += shadow->velocity.getNormalized() * shadow->velocity.getNormalized();

		    v3 frictionForce = 
			    -shadow.velocity.getNormalized() * pow(shadow.velocity.getLength(), shadow.friction_power)*shadow.friction_k;

		    v3 newVelocity = shadow.velocity + timeInfo.frameLength*frictionForce/shadow.mass;

		    if ((newVelocity & shadow.velocity) > 0)
		    {
			    shadow.velocity = newVelocity;
		    }
		    else
		    {
			    shadow.velocity.loadZero();
		    }

            v3 shift = shadow.velocity*timeInfo.frameLength;

//            float mink = 10;
//            Plane collisionTriangle;
//            bool wasCollision = false;
//
//            for EACH_CONST(PolyhedraShadowVector, polyhedras, polyhedra)
//            {
//                for EACH_CONST(PlaneVector, (*polyhedra)->triangles, triangleIt)
//                {
//                    const Plane& triangle = *triangleIt;
//
//                    float k;
//                    if (isCrossTrgLine(triangle, Line(shadow->position, shift), k) && k >= 0 && k <= mink && k <= 1 + EPSILON)
//                    {
//                        mink = k;
//                        collisionTriangle = triangle;
//                        wasCollision = true;
//                    }
//                }
//            }
//
//            if (wasCollision)
//            {
////                if (shift.getSquaredLength() > EPSILON2)
//                {
//        		    shadow->position += shift*mink;
//                }
//
//                shadow->velocity = collisionTriangle.reflect(shadow->velocity);
//                shadow->velocity.loadZero(); // TODO: physic
//                shadow->position += collisionTriangle.a.crossProduct(collisionTriangle.b).getNormalized() * 10 * EPSILON;
//
//            }
//            else
//            {
    		    shadow.position += shift;
//            }
	    }
	}

	return true;
}

void PhysicEngine::setSpeedup(float _speedup)
{
    speedup = _speedup;
}


bool PhysicEngine::clear()
{
	while (!objects.empty())
	{
		remove(objects.back());
	}

	return true;
}

bool PhysicEngine::init(Config& conf)
{
    globalGravity = conf.getv3("global_gravity");

    return true;
}

PhysicEngine::PhysicEngine():
	currentShadow(NULL),
    currentParticleShadow(NULL),
    currentPolyhedraShadow(NULL),
	Engine(),
    globalGravity(v3(0, 0, -1))
{
}
