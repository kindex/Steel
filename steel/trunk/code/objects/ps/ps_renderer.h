﻿/*id*********************************************************
	File: objects/ps/ps_renderer.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Несколько вариантов рендерера для системы частиц
************************************************************/
#ifndef __PARICLE_SYSTEM__RENDERER_H
#define __PARICLE_SYSTEM__RENDERER_H

#include "particle_system.h"
#include "../model_obj.h"

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
	Material	*m;
	Vertexes	vertex;
	FaceMaterials	face;
	TexCoords	texCoords;
	Normals		normal;
	v3			eye;
	SpriteAlign	align;
	v3			customAlign;
public:
	void initSprites(void);
	void initSprites(int begin, int end);
	bool initParticles(void);


	Vertexes*	getVertexes(void) { return &vertex; }
	FaceMaterials* getFaceMaterials() { return &face; }
	Normals*	getNormals(void) { return &normal; }
	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }

	aabb getFrame();

	ObjectPosition getPosition(void) { return particleSystem->getPosition(); }
	PositionKind getPositionKind(void) { return POSITION_GLOBAL; }
	bool cleanup(void) { return true;}
	GLines*	getLines(void){ return NULL; }
	Lights*	getLights(void) { return NULL;}

	void ProcessGraph(const GraphEngineInfo &info);
};

// каждая частица - отдельная модель (объект)
class ObjectPSRenderer: public ParticleRenderer
{
protected:
	steel::vector<GameObjModel*> children;

public:
	bool initParticles();
	
	int getGraphChildrenCount(void) { return children.size(); }
	GraphObject* getGraphChildren(int i) { return children[i]; }
	ModificationTime getChildrenModificationTime(void) { return set->modificationTime; }

	Vertexes*	getVertexes(void) { return NULL; }
	FaceMaterials* getFaceMaterials(void) { return NULL; }
	Normals*	getNormals(void) { return NULL; }
	TexCoords*	getTexCoords(int texNumber) { return NULL; }

	aabb getFrame(void) { return aabb(); } // TODO

	ObjectPosition getPosition(void) { return particleSystem->getPosition(); }
	PositionKind getPositionKind(void) { return POSITION_GLOBAL; }
	bool cleanup(void) { return true;}
	GLines*	getLines(void){ return NULL; }
	Lights*	getLights(void) { return NULL;}

	void ProcessGraph(const GraphEngineInfo &info);
};

// ничего не рисует
class DummyPSRenderer: public ParticleRenderer
{
protected:
public:
	bool initParticles(void) { return true; }
	Vertexes*	getVertexes(void) { return NULL; }
	FaceMaterials* getFaceMaterials(void) { return NULL; }
	Normals*	getNormals(void) { return NULL; }
	TexCoords*	getTexCoords(int texNumber) { return NULL; }

	aabb getFrame(void) { return aabb(); } // TODO

	ObjectPosition getPosition(void) { return particleSystem->getPosition(); }
	PositionKind getPositionKind(void) { return POSITION_GLOBAL; }
	bool cleanup(void) { return true;}
	GLines*	getLines(void){ return NULL; }
	Lights*	getLights(void) { return NULL;}
};

#endif
