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

#include "../engine/game_object.h"
#include "../res/model/model.h"


/*
Рисуемый объект, GraphMesh и PhysicMesh берутся из модели
Может иметь детей.
*/
class GameObjectModel: public GameObject
{
protected:
	Model *m;
	Config *conf;
	FaceMaterials faceMaterial;

public:
	GameObjectModel()	{ 		m = NULL; conf = NULL;	}

	void assignModel(Model *M)	{		m = M;	}

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
	Config*		getPMaterial()
	{	
		return conf;
	}
};

#endif
