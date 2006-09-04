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

#include "graph_engine.h"

bool GraphEngine::inject(GraphObject *object)
{
	// ���� ������ �� ����� �����������
	if(!object->GraphBeforeInject()) return false;
	// ������ ���������� ��������
	objects.push_back(object);
	// �������� ������
	makeStorageForObject(object);
//	cacheStroageObject(getStorage(object));
	makeStorageForChildren(object);

	return true;
}


bool GraphEngine::clear()
{
	while(!objects.empty())
	{
		remove(objects.back());
	}

	return true;
}

bool GraphEngine::remove(GraphObject *object)
{
	deleteStorageForChildren(idHash[object->getId()]);
	deleteStorageForObject(idHash[object->getId()]);
	
	for(steel::vector<GraphObject*>::iterator it = objects.begin(); it != objects.end(); it++)
		if(*it == object)
		{
			objects.erase(it);
			break;
		}
	object->GraphAfterRemove();

	return true;
}


void Camera::setup(const v3 &EYE, const v3 &DIR)
{
	eye = EYE;
	center = EYE + DIR;
}

Camera::Camera(): 
		up(v3(0.0, 0.0, 1.0)), 
		eye(10.0, 10.0, 1.0), 
		center(v3(0.0, 0.0, 0.0)) 
{
}
