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
protected:
	Material*	material;
	Vertexes	vertexes;
	FaceMaterials	face;
	TexCoords	texCoords;
	Normals		normals;
	v3			cameraPosition;
	SpriteAlign	align;
	v3			customAlign;
public:
	void initSprites();
	void initSprites(int begin, int end);
	bool initParticles();

	void process(IN const ProcessInfo& info);
	bool updateInformation(InterfaceId, Engine*);
	AABB getFrame();
};

// каждая частица - отдельная модель (объект)
class ObjectPSRenderer: public ParticleRenderer
{
protected:
	pvector<GameObject*> children;

public:
	bool initParticles();
};

// ничего не рисует
class DummyPSRenderer: public ParticleRenderer
{
protected:
public:
	bool initParticles() { return true; }

};

#endif
