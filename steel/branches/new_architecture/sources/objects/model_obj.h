/*id*********************************************************
	File: objects/model_obj.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Статическая модель, котоаря рисуется и движется
 ************************************************************/

#ifndef __GAME_MODEL_OBJ_H
#define __GAME_MODEL_OBJ_H

#include "../steel.h"
#include "game_obj.h"

/*
Рисуемый объект, GraphMesh и PhysicMesh берутся из модели
Может иметь детей.
*/
class GameObjModel: public GameObj
{
protected:
	Model *m;
	OldConfig *conf;
	FaceMaterials faceMaterial;

public:
	GameObjModel()	{ 		m = NULL; conf = NULL;	}

	CollisionType getCollisionType() { return COLLISION_POLYHEDRA; }

	void assignModel(Model *M)	{		m = M;	}
	bool init(ScriptLine	&s);

	aabb getFrame();
	Vertexes*	getVertexes()
	{
		if(m)
			return &m->vertex;
		else
			return NULL;
	}
	Vertexes*	getPVertexes() { return getVertexes(); }
	Triangles*	getTriangles()
	{
		if(m)
			return &m->triangleAll;
		else
			return NULL;
	}

	Normals* getNormals()
	{
		if(m)
			return &m->normal;
		else
			return NULL;
	}
	FaceMaterials* getFaceMaterials();
	Lights* getLights()	{		return NULL;	}

	TexCoords*	getTexCoords(int mapNumber)
	{
		return m->getTexCoords(mapNumber);
	}
	OldConfig*		getPMaterial()
	{	
		return conf;
	}
};

#endif
