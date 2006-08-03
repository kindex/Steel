/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		����������� ������. ������� ���������� ������ � ���� 3d-mesh
		� ������� ������������ ������ (OpenGL_Engine) ����������
		������������ ����� ������� �� ������, �������� �������������
		���������, �������������� ����. ������.
 ************************************************************/

#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../res/conf/conf.h"
#include "graph_interface.h"

class GraphEngine;

class Camera
{
public:
    v3 up, eye, center;

public:
    void seteye(const v3 &EYE) { eye = EYE; }
    void setcenter(const v3 &CENTER) { center = CENTER; }
	void setup(const v3 &EYE, const v3 &DIR);

	Camera();
};

class GraphEngine: public Engine
{
protected:
	steel::vector<GraphObject*> objects;

	typedef std::map <uid, int> StorageHash;
	// ����������� ��������������� ������� �� ��������� � ������� storage
	StorageHash idHash;

	GraphEngineInfo info;
public:
	struct GraphTotalInfo
	{
		int vertex, triangle, object, global;
	} total;

public:
	Camera camera;
	virtual void processCamera() = 0;
	// Collect information about object: how to render it
	virtual bool inject(GraphObject *object);
	virtual bool remove(GraphObject *object);
	
	virtual bool process(steel::time globalTime, steel::time time) = 0;

	// Draw colelcted information. May be called few times without recollection information
	virtual bool isVisible(aabb box) = 0;
	// Clear collected information
	bool clear();
	
		// ������ ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ������ �������
	virtual void makeStorageForObject(GraphObject *object) = 0;
	virtual void deleteStorageForObject(int sid)  = 0;
	// ������ ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ����� �������
	virtual void makeStorageForChildren(GraphObject *object)  = 0;
	virtual void deleteStorageForChildren(int sid)  = 0;
};

#endif
