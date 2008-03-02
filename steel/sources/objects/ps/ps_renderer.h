/*id*********************************************************
	File: objects/ps/ps_renderer.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Несколько вариантов рендерера для системы частиц
************************************************************/
#ifndef __PARICLE_SYSTEM__RENDERER_H
#define __PARICLE_SYSTEM__RENDERER_H

#include "particle_system.h"
#include "../../graph/material.h"
#include "../../graph/graph_interface.h"
#include "../../math/sprite.h"
#include "../combiner/graph_object.h"

// система - модель из множества полигонов - спрайтов
class SpriteRenderer: public ParticleRenderer
{
public:
	bool beforeInject(IN OUT Engine&) { return true; }
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id);
	void bindEngine(IN OUT Engine&);
	bool InitFromConfig(IN Config&);
	void process(IN const ProcessInfo& info) {}
	void onParticleBorn(int index) {}
	void onParticleDie(int index) {}

	void updateSpritePositions(IN const ProcessInfo&);
	void initSprites();
	void initSprites(int begin, int end);
	bool initParticles();
	AABB getFrame();

private:
	MaterialStd*	   material;
	VertexVector	   vertexes;
	TexCoords		   texCoords;
	Normals			   normals;
	v3				   cameraPosition;
	SpriteAlign		   align;
	v3				   customAlign;
	FaceMaterialVector face;
};

// каждая частица - отдельная модель (объект)
class ObjectPSRenderer: public ParticleRenderer
{
public:
	bool initParticles();
	bool InitFromConfig(Config&);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo& info){}
	void onParticleBorn(int index);
	void onParticleDie(int index) {}

protected:
    ConfigStruct* particle_config;
    struct Object
    {
        Object(bool inserted, GraphObject* object) : 
            inserted(inserted),
            object(object)
        {}

        bool         inserted;
        GraphObject* object;
    };
    std::vector<Object> objects;
};

// ничего не рисует
class DummyPSRenderer: public ParticleRenderer
{
public:
	bool initParticles() { return true; }
	bool InitFromConfig(Config&) { return true;}
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id) { return false; }
	void process(IN const ProcessInfo& info){}
	void onParticleBorn(int index) {}
	void onParticleDie(int index) {}
};

#endif
