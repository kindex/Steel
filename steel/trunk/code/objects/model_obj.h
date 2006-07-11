/*id*********************************************************
    Unit: objects/model
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Статическая модель, котоаря рисуется и движется
 ************************************************************/

#ifndef __GAME_MODEL_OBJ_H
#define __GAME_MODEL_OBJ_H

#include "game_obj.h"

/*
Рисуемый объект, GraphMesh и PhysicMesh берутся из модели
Может иметь детей.
*/
class GameObjModel: public GameObj
{
protected:
	Model *m;
	Config *conf;
	FaceMaterials faceMaterial;

public:
	GameObjModel()	{ 		m = NULL; conf = NULL;	}

	CollisionType::CollisionType getCollisionType() { return CollisionType::polyhedra; }

	void assignModel(Model *M)	{		m = M;	}
	bool init(ScriptLine	&s);

	aabb getFrame();
	Vertexes*	GetVertexes()
	{
		if(m)
			return &m->vertex;
		else
			return NULL;
	}
	Vertexes*	getPVertexes() { return GetVertexes(); }
	Triangles*	getTriangles()
	{
		if(m)
			return &m->triangleAll;
		else
			return NULL;
	}

	Normals* GetNormals()
	{
		if(m)
			return &m->normal;
		else
			return NULL;
	}
	FaceMaterials* GetFaceMaterials();
	Lights* GetLights()	{		return NULL;	}

	TexCoords*	GetTexCoords(int mapNumber)
	{
		return m->GetTexCoords(mapNumber);
	}
	Config*		getPMaterial()
	{	
		return conf;
	}
};

#endif
