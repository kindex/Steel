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
	void initSprites();
	void initSprites(int begin, int end);
	bool initParticles();


	Vertexes*	getVertexes() { return &vertex; }
	FaceMaterials* getFaceMaterials() { return &face; }
	Normals*	getNormals() { return &normal; }
	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }

	aabb getFrame();

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind getPositionKind() { return POSITION_GLOBAL; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}

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

	Vertexes*	getVertexes() { return NULL; }
	FaceMaterials* getFaceMaterials() { return NULL; }
	Normals*	getNormals() { return NULL; }
	TexCoords*	getTexCoords(int texNumber) { return NULL; }

	aabb getFrame() { return aabb(); } // TODO

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind getPositionKind() { return POSITION_GLOBAL; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}

	void ProcessGraph(const GraphEngineInfo &info);
};

// ничего не рисует
class DummyPSRenderer: public ParticleRenderer
{
protected:
public:
	bool initParticles() { return true; }
	Vertexes*	getVertexes() { return NULL; }
	FaceMaterials* getFaceMaterials() { return NULL; }
	Normals*	getNormals() { return NULL; }
	TexCoords*	getTexCoords(int texNumber) { return NULL; }

	aabb getFrame() { return aabb(); } // TODO

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind getPositionKind() { return POSITION_GLOBAL; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}
};


#endif
