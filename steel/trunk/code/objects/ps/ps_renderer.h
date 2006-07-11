#ifndef __PARICLE_SYSTEM__RENDERER_H
#define __PARICLE_SYSTEM__RENDERER_H

#include "particle_system.h"
#include "../model_obj.h"

namespace SpriteAlign
{
	typedef	enum
	{
		camera,
		z,
		screen,
		custom
	} SpriteAlign;
}

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
	SpriteAlign::SpriteAlign	align;
	v3			customAlign;
public:
	void initSprites();
	void initSprites(int begin, int end);
	bool initParticles();


	Vertexes*	GetVertexes() { return &vertex; }
	FaceMaterials* GetFaceMaterials() { return &face; }
	Normals*	GetNormals() { return &normal; }
	TexCoords*	GetTexCoords(int texNumber) { return &texCoords; }

	aabb getFrame();

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	GetLines(){ return NULL; }
	Lights*	GetLights() { return NULL;}

	void ProcessGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime, v3	cameraEye, v3 cameraDirection);
};

// каждая частица - отдельная модель (объект)
class ObjectPSRenderer: public ParticleRenderer
{
protected:
	steel::vector<GameObjModel*> children;

public:
	bool initParticles();
	
	int GetGraphChildrenCount(void) { return children.size(); }
	GraphObject* GetGraphChildren(int i) { return children[i]; }
	ModificationTime getChildrenModificationTime(void) { return set->modificationTime; }

	Vertexes*	GetVertexes() { return NULL; }
	FaceMaterials* GetFaceMaterials() { return NULL; }
	Normals*	GetNormals() { return NULL; }
	TexCoords*	GetTexCoords(int texNumber) { return NULL; }

	aabb getFrame() { return aabb(); } // TODO

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	GetLines(){ return NULL; }
	Lights*	GetLights() { return NULL;}

	void ProcessGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime, v3	cameraEye, v3 cameraDirection);
};

// ничего не рисует
class DummyPSRenderer: public ParticleRenderer
{
protected:
public:
	bool initParticles() { return true; }
	Vertexes*	GetVertexes() { return NULL; }
	FaceMaterials* GetFaceMaterials() { return NULL; }
	Normals*	GetNormals() { return NULL; }
	TexCoords*	GetTexCoords(int texNumber) { return NULL; }

	aabb getFrame() { return aabb(); } // TODO

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	GetLines(){ return NULL; }
	Lights*	GetLights() { return NULL;}
};


#endif
