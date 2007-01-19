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

typedef	enum
{
	SPRITE_ALIGN_CAMERA,
	SPRITE_ALIGN_Z,
	SPRITE_ALIGN_SCREEN,
	SPRITE_ALIGN_CUSTOM
} SpriteAlign;

// система - модель из множества полигонов - спрайтов
class SpriteRenderer: public ParticleRenderer
{
public:
	bool beforeInject(IN OUT Engine&) { return true; }
	void afterRemove(IN OUT Engine&) {}
	bool updateInformation(IN OUT Engine&);
	void bindEngine(IN OUT Engine&);
	void process(IN const ProcessInfo&);
	bool InitFromConfig(IN Config&);

	void initSprites();
	void initSprites(int begin, int end);
	bool initParticles();
	AABB getFrame();

private:
	Material*	material;
	Vertexes	vertexes;
	FaceMaterials	face;
	TexCoords	texCoords;
	Normals		normals;
	v3			cameraPosition;
	SpriteAlign	align;
	v3			customAlign;
};

// каждая частица - отдельная модель (объект)
class ObjectPSRenderer: public ParticleRenderer
{
public:
	bool initParticles();
	bool InitFromConfig(Config&) { return true;}
	bool updateInformation(IN OUT Engine&) { return false; }
	void process(IN const ProcessInfo& info){}
	bool beforeInject(IN OUT Engine&) { return true; }
	void afterRemove(IN OUT Engine&) {}

protected:
	pvector<GameObject*> children;
};

// ничего не рисует
class DummyPSRenderer: public ParticleRenderer
{
public:
	bool initParticles() { return true; }
	bool InitFromConfig(Config&) { return true;}
	bool updateInformation(IN OUT Engine&) { return false; }
	void process(IN const ProcessInfo& info){}
	bool beforeInject(IN OUT Engine&) { return true; }
	void afterRemove(IN OUT Engine&) {}
};

#endif
