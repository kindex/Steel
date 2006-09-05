/*id*********************************************************
	File: graph/opengl/opengl_engine.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		Графический движок реализлванный на фунциях OpenGL. 
		Рисует трехмерный объект в виде 3d-mesh,
		расчитывает динамическое освещение
 ************************************************************/

#include "../../steel.h"
#include "opengl_engine.h"

#include <iostream>

#include "../../common/utils.h"
#include "../../common/logger.h"

#include "../../res/image/image.h"
#include "gl/libext.h"
#include "ext/normalisation_cube_map.h"
#include "../../res/res_main.h"

#include <algorithm>

using namespace std;

/*
Сердце Графического движка.
Отвечает за вывод графичесткого элемента.
*/


void OpenGL_Engine::process(GraphObjectStorage &e, steel::time globalTime, steel::time time)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float m[16]; // TODO
	m[0] = e.matrix.data.matrix.data.m[0][0];	m[1] = e.matrix.data.matrix.data.m[1][0];	m[2] = e.matrix.data.matrix.data.m[2][0];	m[3]  = 0;
	m[4] = e.matrix.data.matrix.data.m[0][1];	m[5] = e.matrix.data.matrix.data.m[1][1];	m[6] = e.matrix.data.matrix.data.m[2][1];	m[7]  = 0;
	m[8] = e.matrix.data.matrix.data.m[0][2];	m[9] = e.matrix.data.matrix.data.m[1][2];	m[10] = e.matrix.data.matrix.data.m[2][2];	m[11] = 0;
	m[12] = e.matrix.data.vector.x;				m[13] = e.matrix.data.vector.y;				m[14] = e.matrix.data.vector.z;				m[15] = 1;
	glLoadMatrixf(m);

	if(conf->geti("drawFace") && e.faceMaterials && DrawFill)
		for(unsigned int i = 0; i < e.faceMaterials->size(); i++)
			(this->*DrawFill)(e, e.faceMaterials->at(i).triangles, e.faceMaterials->at(i).material, total);

	if(conf->geti("drawWire") && e.faceMaterials && DrawWire)
		for(unsigned int i = 0; i < e.faceMaterials->size(); i++)
			(this->*DrawWire)(e, e.faceMaterials->at(i).triangles, total);

	if(conf->geti("drawLines") && DrawLines)
		(this->*DrawLines)(e, total);

	if(conf->geti("drawNormals") && DrawNormals)
		(this->*DrawNormals)(e, total);

	if(conf->geti("drawVertexes") && DrawVertexes)
		(this->*DrawVertexes)(e, total);

	if(conf->geti("drawAABB") && DrawAABB)
		(this->*DrawAABB)(e, total);

	glPopMatrix();
}



bool OpenGL_Engine::isVisible(aabb box)
{
/*	matrix34 proj; TODO
	glGetFloatv(GL_PROJECTION_MATRIX, proj.a);
	box.mul(proj);

	return box.intersect(aabb(v3(-1.1,-1.1,0), v3(1.1,1.1,2)));*/
	return true;
}

/*void OpenGL_Engine::cleanCache()
{
	for(map<uid, OpenGL_Buffer>::iterator it = buffer.begin(); it != buffer.end(); it++)
	{
		uid id = it->first;
		OpenGL_Buffer &buf = it->second;
		if(buf.temp)
		{

		}
	}
}*/

bool OpenGL_Engine::process(steel::time globalTime, steel::time time)
{
	// TODO repair DC 

	lights.clear();
	total.vertex = 0;
	total.triangle = 0;
	total.object = 0;

	int size = objects.size();

	for(int i=0; i < size; i++)
		prepare(objects[i], globalTime, time); /* Update vertexes, faces, ights */


//	if(!ARB_multitexture_supported) 
//		conf->setup("drawBump", 0);

	GLbitfield clear = 0;
	if(conf->geti("clearColor", 1))	clear |= GL_COLOR_BUFFER_BIT;
	if(conf->geti("clearDepth", 1))	clear |= GL_DEPTH_BUFFER_BIT;
	if(clear)
		glClear(clear);

/*
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glLightfv( GL_LIGHT0, GL_POSITION, v4( camera.center ));*/


//	steel::vector<int> elementAlpha;

// В начале выводим только непрозрачные объекты
	for(steel::vector<GraphObjectStorage>::iterator it = storage.begin(); it != storage.end(); it++)
//		if(!it->blend)
			process(*it, globalTime, time);
	/*		else
		{
			it->distance = (camera.eye - it->matrix*v3(0,0,0)).getLength();
			elementAlpha.push_back(*it);
		}
*/

// Потом прозрачные в порядке удалённости от камеры: вначале самые дальние

/*	if(conf->geti("drawAlpha")>0)
	{
		sort(elementAlpha.begin(), elementAlpha.end());

		for(steel::vector<DrawElement>::iterator it = elementAlpha.begin(); it != elementAlpha.end(); it++)
			drawElement((*it));
	}
	elementAlpha.clear();
*/
	if(conf->geti("swapBuffers", 1))
	{
		glFlush(); // TODO: flush in thread
		swapBuffers();
	}

	return true;
}

bool OpenGL_Engine::init(std::string _conf)
{
	if(!(conf = resConfig.add(_conf)))
	{
		log_msg("error graph conf res","Cannot find renderer config file "+_conf);
		return false;
	}

	conf->setDefault("window.left", "10");
	conf->setDefault("window.top", "10");

	conf->setDefault("window.width", "800");
	conf->setDefault("window.height", "600");

	conf->setDefault("screen.depth", "32");
	conf->setDefault("screen.width", "800");
	conf->setDefault("screen.height", "600");

	conf->setDefault("fullscreen", "0");

	if (!createWindow())
	{
		//lastError = getError();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.01, 1.0e+6);

	glDepthFunc(GL_LESS); 
	glCullFace(GL_BACK);

//    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	log_msg("graph opengl opengl_info", "Renderer: " + (string)(const char *)glGetString(GL_RENDERER));
	log_msg("graph opengl opengl_info", "Version: " + (string)(const char *)glGetString(GL_VERSION));
	log_msg("graph opengl opengl_info", "Vendor: " + (string)(const char *)glGetString(GL_VENDOR));

	string openglVersions = (const char *)glGetString(GL_VERSION);
	int openglVersioni;
	
	if(openglVersions.length() >= 3)
		openglVersioni = (openglVersions[0] - '0')*10 + (openglVersions[2] - '0');
	else
	{
		log_msg("graph opengl error opengl_info", "Could not detect OpenGL version. Using 1.0");
		openglVersioni = 10;
	}

	log_msg("graph opengl opengl_info", "Detected OpenGL version code: " + IntToStr(openglVersioni));

	OpenGL_ExtensionsInit();
	OpenGL_ExtensionsPrintfInfo();

	int version = conf->geti("OpenGL_Version", 0);
	
	if(version == 0)// if version is not set in config, then autodetect it
	{
		version = openglVersioni;
	}

	log_msg("graph opengl opengl_info", "Using OpenGL renderer version: " + IntToStr(version));

	BindTexture = &OpenGL_Engine::BindTexture_OpenGL10;
	DrawFill = &OpenGL_Engine::DrawFill_OpenGL10;
	DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL10;
	DrawWire = &OpenGL_Engine::DrawWire_OpenGL10;
	DrawLines = &OpenGL_Engine::DrawLines_OpenGL10;
	DrawNormals = &OpenGL_Engine::DrawNormals_OpenGL10;
	DrawVertexes = &OpenGL_Engine::DrawVertexes_OpenGL10;
	DrawAABB = &OpenGL_Engine::DrawAABB_OpenGL10;

	if(version >= 11)
	{
		BindTexture = &OpenGL_Engine::BindTexture_OpenGL11;
		DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL11;
		DrawWire = &OpenGL_Engine::DrawWire_OpenGL11;
		DrawLines = &OpenGL_Engine::DrawLines_OpenGL11;
		BindTexCoords = &OpenGL_Engine::BindTexCoords_OpenGL11;
		BindTexCoords3f = &OpenGL_Engine::BindTexCoords3f_OpenGL11;
	}

	if(version >= 13)
	{
		if(GL_EXTENSION_MULTITEXTURE)
			DrawFill = &OpenGL_Engine::DrawFill_OpenGL13;
	}

	if(version >= 15)
	{
		if(GL_EXTENSION_VBO)
		{
			DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL15;
			BindTexCoords = &OpenGL_Engine::BindTexCoords_OpenGL15;
			BindTexCoords3f = &OpenGL_Engine::BindTexCoords3f_OpenGL15;
		}
	}

	if(version >= 20)
	{
		if(GL_EXTENSION_GLSL)
			DrawFill = &OpenGL_Engine::DrawFill_OpenGL20;
	}


	normalisationCubeMap	= generateNormalisationCubeMap();
	zeroNormal = resImage.add("zero");
	if(!zeroNormal)
	{
		log_msg("error graph res", "Zero normal map not found");
		return false;
	}

	log_msg("opengl graph", "OpenGL engine has been initialized!");

	setCaption("Steel Engine");

	clear();

	return true;
}

bool OpenGL_Engine::deinit()
{
	log_msg("opengl graph", "OpenGL engine has been stopped!");
	return true;
}


void OpenGL_Engine::processCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
//	gluPerspective( camera.fov, (float)window.width/window.height, front, back );

	conf->setup("camera.aspect", conf->getd("window.width") / conf->getd("window.height"));

	gluPerspective(conf->getd("camera.fov"), conf->getd("camera.aspect"), conf->getd("camera.min_dist"), conf->getd("camera.max_dist"));

    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, 
			camera.center.x, camera.center.y, camera.center.z, 
			camera.up.x, camera.up.y, camera.up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

OpenGL_Engine::GraphObjectStorage &OpenGL_Engine::getStorage(GraphObject *object)
{
	uid id = object->getId();
	assert(idHash.find(id) != idHash.end(), "Object not found in graph storage");

	return storage[idHash[id]];
}

void OpenGL_Engine::prepare(GraphObject *object, steel::time globalTime, steel::time time, matrix34 matrix, GraphObject *parent)
{

	info.curTime = globalTime;
	info.frameLength = time;
	info.modificationTime = globalFrameNumber;
	info.cameraEye = camera.eye;
	info.cameraDirection = camera.center - camera.eye;

	object->ProcessGraph(info);

	int sid = idHash[object->getId()];
		
	cacheStorageObject(storage[sid]);

	if(storage[sid].childrenModificationTime < object->getChildrenModificationTime())
	{
		storage[sid].childrenModificationTime = object->getChildrenModificationTime();

		StorageHash newChildrenId;

		int count = object->getGraphChildrenCount();
		for(int i = 0; i < count; i++) // add new + cache
		{
			GraphObject *child = object->getGraphChildren(i);
			uid id = child->getId();
			newChildrenId[id] = i;

			if(idHash.find(id) == idHash.end())
			{
				// add new object to storage
				makeStorageForObject(child);
				makeStorageForChildren(child);

				storage[sid].children.push_back(id);
			}
			cacheStorageObject(getStorage(child));
		}
		int size = storage[sid].children.size();

		for(int i = 0; i<size; i++)
		{
			uid id = storage[sid].children[i];
			if(newChildrenId.find(id) == newChildrenId.end())
			{
				int n = idHash[id];
				deleteStorageForChildren(n);
				deleteStorageForObject(n);
				storage[sid].children[i] = storage[sid].children.back();
				storage[sid].children.pop_back();
				size--;
				i--;
			}
		}
	}

	if(storage[sid].lights)
	for(Lights::const_iterator it = storage[sid].lights->begin(); it != storage[sid].lights->end(); it++)
	{
		lights.push_back(*it);
		lights.back().position = storage[sid].matrix*lights.back().position;
	}

	int count = object->getGraphChildrenCount();
	for(int i = 0; i < count; i++)
	{
		GraphObject *child = object->getGraphChildren(i);
		
		prepare(child, globalTime, time);
	}

}

void OpenGL_Engine::cacheStorageObject(GraphObjectStorage &objectStorage)
{
	GraphObject *object = objectStorage.object;

	matrix34 object_matrix = object->getPosition(); // global 
	PositionKind pos = object->getPositionKind();

	objectStorage.matrix = object_matrix;

/*	if(pos == POSITION_LOCAL && parent)
	{
		object_matrix = parent_matrix*object_matrix;
	}*/


	if(objectStorage.modificationTime < object->getModificationTime())
	{
		objectStorage.modificationTime = object->getModificationTime();
//		aabb frame = object->getFrame();
//		frame.mul(object_matrix);

// проверка, находится ли frame внутри пирамиды, которую образует угол обзора камеры. Если не попадает, то откидываем этот объект и всех его потомков
//		objectStorage.visible = isVisible(frame);

		objectStorage.faceMaterials	= object->getFaceMaterials();
		objectStorage.vertex		= object->getVertexes();
		objectStorage.normal		= object->getNormals();
		objectStorage.lines			= object->getLines();
		objectStorage.lights		= object->getLights();
		objectStorage.blend			= false;
	}
}

void OpenGL_Engine::makeStorageForObject(GraphObject *object)
{
	uid objectId = object->getId();
	if(idHash.find(objectId) != idHash.end())
	{
		log_msg("error graph", "Duplicate object " + IntToStr(objectId) + " in storage");
		return;
	}

	int storageId = storage.size();
	storage.resize(storageId + 1);

	GraphObjectStorage &objectStorage = storage[storageId];

	idHash[objectId] = storageId;

	objectStorage.object = object;
	objectStorage.storageId = storageId;
	objectStorage.objectId = objectId;

	objectStorage.modificationTime = -1;
	objectStorage.childrenModificationTime = -1;
}

void OpenGL_Engine::deleteStorageForObject(int sid)
{
	idHash.erase(storage[sid].objectId);
	storage[sid] = storage.back();
	idHash[storage[sid].objectId] = sid;
	storage.pop_back();
}

void OpenGL_Engine::deleteStorageForChildren(int sid)
{
	int count = storage[sid].children.size();
	for(int i = 0; i < count; i++)
	{
		int n = idHash[storage[sid].children[i]];
		deleteStorageForChildren(n);
		deleteStorageForObject(n);
	}
}


void OpenGL_Engine::makeStorageForChildren(GraphObject *object)
{
	int count = object->getGraphChildrenCount();
	for(int i = 0; i < count; i++)
	{
		GraphObject *child = object->getGraphChildren(i);
		makeStorageForObject(child);
		makeStorageForChildren(child);
	}
}
