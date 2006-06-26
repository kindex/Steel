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
	bool	init(ScriptLine	&s);

	aabb getFrame();
	bool cleanup()	{	return true;		}
	bool cleanupP()	{	return cleanup();	}
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
