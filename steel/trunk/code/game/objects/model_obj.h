#ifndef __GAME_MODEL_OBJ_H
#define __GAME_MODEL_OBJ_H

#include "../game_obj.h"

/*
Рисуемый объект, GraphMesh и PhysicMesh берутся из модели
Может иметь детей.
*/
class GameObjModel: public GameObj
{
protected:
	Model *m;

public:
	void assignModel(Model *M)
	{
		m = M;
	}
	GameObjModel()
	{ 
		m = NULL;
	}
	bool	init(ScriptLine	&s, ResCollection &res);


	uid		getId() { return m->getId(); }
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
	FaceMaterials* getFaceMaterials()
	{
		if(m)
			return &m->faceMaterial;
		else
			return NULL;
	}
	Lights* getLights()
	{
		return NULL;
	}

	TexCoords*	getTexCoords(int mapNumber)
	{
		return m->getTexCoords(mapNumber);
	}
//	bool	init(ScriptLine	&line)
};


#endif
