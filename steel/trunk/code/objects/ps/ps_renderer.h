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

	void processGraph(v3 cameraEye, v3 cameraDirection);
};

// каждая частица - отдельная модель (объект)
class ObjectPSRenderer: public ParticleRenderer
{
protected:
	GraphObjectList children;
	steel::svector<GameObjModel*> childrenModel;

public:
	bool initParticles();
	
	virtual GraphObjectList* getGraphChildrenList(void) { return &children; }

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

	void processGraph(v3 cameraEye, v3 cameraDirection);
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
	PositionKind::PositionKind getPositionKind() { return PositionKind::global; }
	bool cleanup() { return true;}
	GLines*	getLines(){ return NULL; }
	Lights*	getLights() { return NULL;}

	void processGraph(v3 cameraEye, v3 cameraDirection) {}
};


#endif
