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

// ������� - ������ �� ��������� ��������� - ��������
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


	Vertexes*	getVertexes() { return &vertex; }
	FaceMaterials* getFaceMaterials() { return &face; }
	Normals*	getNormals() { return &normal; }
	TexCoords*	getTexCoords(int texNumber) { return &texCoords; }

	aabb getFrame();

	ObjectPosition getPosition() { return particleSystem->getPosition(); }
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}

	void ProcessGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime, v3	cameraEye, v3 cameraDirection);
};

// ������ ������� - ��������� ������ (������)
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
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}

	void ProcessGraph(steel::time curTime, steel::time frameLength, ModificationTime modificationTime, v3	cameraEye, v3 cameraDirection);
};

// ������ �� ������
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
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}
};


#endif
