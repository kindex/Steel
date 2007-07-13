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

#ifndef _PHYSIC_PARTICLE_H_
#define _PHYSIC_PARTICLE_H_
#include "../steel.h"
#include "physic_interface.h"
#include "../engine/engine_types.h"

typedef pvector<Plane> PlaneVector;

class PhysicEngine: 
    public Engine, 
    public PhysicInterface
{
	struct ParticleShadow: public Shadow // множество треугольников одного материала
	{
		v3		position;
		v3		velocity;
		v3		force;
		Config*	config;
		bool	enabled;

		float mass;
		float distance_k;
		
		float spring_r0;
		float spring_k;

		float gravity_k;
		float gravity_min_dist;
		float gravity_power;

		float friction_k;
		float friction_power;

		float lj_power1;
		float lj_power2;
        float global_gravity_k;

		ParticleShadow(Engine* aEngine): Shadow(aEngine), enabled(false) {}
	};
    typedef svector<ParticleShadow*> ParticleShadowVector;

    struct PolyhedraShadow: public Shadow // множество треугольников одного материала
	{
	    PositionKind	    positionKind;
	    ObjectPosition	    position; // global or screen
	    ObjectPosition	    realPosition; // global, calculated from position and parent
        bool                realPositionCalculated;

        const VertexVector* vertexes;
        const Faces*        faces;
        PlaneVector         triangles;

		PolyhedraShadow(Engine* aEngine): 
            Shadow(aEngine),
            vertexes(NULL),
            faces(NULL),
            positionKind(POSITION_LOCAL),
            position(matrix34::getIdentity())
        {
        }
	};
    typedef svector<PolyhedraShadow*> PolyhedraShadowVector;

public:
	bool setCurrentObject(GameObject*, IN const InterfaceId);
	void addChild(GameObject* child);
	void addChild(Shadow& shadow, GameObject* child);
	void deleteChild(GameObject* child);
	void clearChildren();
	void setPosition(IN const v3);
	v3	 getPosition();
	void setVelocity(IN const v3);
	v3	 getVelocity();
	void setConfig(Config&);

	void setVertexes(const VertexVector*);
	void setFaces(const Faces*);
	void setPosition(const ObjectPosition&);
	void setPositionKind(PositionKind);

	PhysicEngine();
	bool init(Config&);

	bool clear();
	bool inject(GameObject* object);

	bool remove(GameObject* object);
	bool process(IN const TimeInfo& info);
    void setSpeedup(float speedup);


protected:
    int addShadow(Shadow* newStorage, const InterfaceId);
	void updateRealPosition(IN OUT PolyhedraShadow* object);
	Shadow* shadowClassFactory(GameObject* object, const InterfaceId);
	void prepare(ParticleShadow* shadow, GameObject* parent);
	ParticleShadow* getParticleShadow(uid id);
	PolyhedraShadow* getPolyhedraShadow(uid id);
	bool remove(GameObject* object, const InterfaceId id);

// ***************************** PARTICLE *************************
    v3 calculateForceForParticle(ParticleShadow* shadow1, ParticleShadow* shadow2);
	v3 PhysicEngine::calculateForceForParticle(ParticleShadow* shadow);
	bool processParticle(ParticleShadow* shadow);

    ParticleShadowVector    particles;
    PolyhedraShadowVector   polyhedras;
    Shadow*                 currentShadow;
	ParticleShadow*         currentParticleShadow;
	PolyhedraShadow*        currentPolyhedraShadow;

	pvector<GameObject*>	objects;
	TimeInfo				timeInfo;
    float                   speedup;
    v3                      globalGravity;
};


#endif
