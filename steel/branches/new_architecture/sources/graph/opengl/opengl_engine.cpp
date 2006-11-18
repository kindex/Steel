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


bool OpenGL_Engine::setCurrentObject(GameObject* object)
{
	uid id = object->getId();
	currentStorage = getStorage(id);
	if(currentStorage == NULL)
	{
		currentObject = NULL;
		return false;
	}
	currentObject = object;

	return false;
}

void OpenGL_Engine::setPosition(ObjectPosition position)
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->position = position;
	}
}

void OpenGL_Engine::setPositionKind(PositionKind kind)
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->positionKind = kind;
	}
}


void OpenGL_Engine::addChild(GameObject* parent, GameObject* child)
{
	// TODO:
}

void OpenGL_Engine::deleteChild(GameObject* parent, GameObject* child)
{
	// TODO:
}

void OpenGL_Engine::clearChildren(GameObject*)
{
	// TODO:
}

void OpenGL_Engine::setVertexes(Vertexes* vertexes) // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->vertex = vertexes;
	}
}

void OpenGL_Engine::setNormals(Normals* normals) // список нормалей в вершинам
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->normal = normals;
	}
}
void OpenGL_Engine::setLines(GLines* lines) // индексы вершин для линий и цвета линий (for debug)
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->lines = lines;
	}

}

void OpenGL_Engine::setFaceMaterials(FaceMaterials* faceMaterials)// массив индексов вершин, которые образуют треугольники (грани) + материалы
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->faceMaterials = faceMaterials;
	}
}

void OpenGL_Engine::setTexCoordsCount(int size)
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->texCoords.resize(size);
	}
}

void OpenGL_Engine::setTexCoords(int texNumber, TexCoords* coords)
{
	if(currentStorage != NULL && static_cast<int>(GS(currentStorage)->texCoords.size()) > texNumber)
	{
		GS(currentStorage)->texCoords[texNumber] = coords;
	}
}

void OpenGL_Engine::setLights(Lights* lights)
{
	if(currentStorage != NULL)
	{
		GS(currentStorage)->lights = lights;
	}
}

/*
Сердце Графического движка.
Отвечает за вывод графичесткого элемента.
*/

void OpenGL_Engine::process(GraphShadow *e, steel::time globalTime, steel::time time)
{
	if(e->positionKind == POSITION_SCREEN)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
	}
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	float m[16]; // TODO
	m[0] = e->position.data.matrix.data.m[0][0];	m[1] = e->position.data.matrix.data.m[1][0];	m[2] = e->position.data.matrix.data.m[2][0];	m[3]  = 0;
	m[4] = e->position.data.matrix.data.m[0][1];	m[5] = e->position.data.matrix.data.m[1][1];	m[6] = e->position.data.matrix.data.m[2][1];	m[7]  = 0;
	m[8] = e->position.data.matrix.data.m[0][2];	m[9] = e->position.data.matrix.data.m[1][2];	m[10] = e->position.data.matrix.data.m[2][2];	m[11] = 0;
	m[12] = e->position.data.vector.x;				m[13] = e->position.data.vector.y;				m[14] = e->position.data.vector.z;				m[15] = 1;
	glLoadMatrixf(m);

	if(conf->geti("drawFace") && e->faceMaterials && DrawFill)
		for(unsigned int i = 0; i < e->faceMaterials->size(); i++)
			(this->*DrawFill)(*e, e->faceMaterials->at(i).triangles, e->faceMaterials->at(i).material, total);

	if(conf->geti("drawWire") && e->faceMaterials && DrawWire)
		for(unsigned int i = 0; i < e->faceMaterials->size(); i++)
			(this->*DrawWire)(*e, e->faceMaterials->at(i).triangles, total);

	if(conf->geti("drawLines") && DrawLines)
		(this->*DrawLines)(*e, total);

	if(conf->geti("drawNormals") && DrawNormals)
		(this->*DrawNormals)(*e, total);

	if(conf->geti("drawVertexes") && DrawVertexes)
		(this->*DrawVertexes)(*e, total);

	if(conf->geti("drawAABB") && DrawAABB)
		(this->*DrawAABB)(*e, total);

	glPopMatrix();
	if(e->positionKind == POSITION_SCREEN)
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
		prepare(getStorage(objects[i]), globalTime, time); /* Update vertexes, faces, ights */


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
	for(svector<Shadow*>::iterator it = shadows.begin(); it != shadows.end(); it++)
//		if(!it->blend)
	{
			GraphShadow *storage = GS(*it);
			process(storage, globalTime, time);
	}
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
//		glFlush(); // TODO: flush in thread
		(this->*FlushOpenGL_Window)();
//		swapBuffers();
	}

	return true;
}

#if STEEL_OS == OS_WIN32
void OpenGL_Engine::UseWinAPI(void)
{
	CreateOpenGL_Window = &OpenGL_Engine::CreateOpenGL_Window_WinAPI;
	RepairOpenGL_Window = &OpenGL_Engine::RepairOpenGL_Window_WinAPI;
	DeleteOpenGL_Window = &OpenGL_Engine::DeleteOpenGL_Window_WinAPI;
	setCaptionOpenGL_Window = &OpenGL_Engine::setCaptionOpenGL_Window_WinAPI;
	FlushOpenGL_Window = &OpenGL_Engine::FlushOpenGL_Window_WinAPI;
	log_msg("graph opengl opengl_info", "Using VideoAPI::WinAPI");
}
#endif

#ifdef LIB_SDL
void OpenGL_Engine::UseSDL(void)
{
	CreateOpenGL_Window = &OpenGL_Engine::CreateOpenGL_Window_SDL;
	RepairOpenGL_Window = NULL;
	DeleteOpenGL_Window = &OpenGL_Engine::DeleteOpenGL_Window_SDL;
	setCaptionOpenGL_Window = &OpenGL_Engine::setCaptionOpenGL_Window_SDL;
	FlushOpenGL_Window = &OpenGL_Engine::FlushOpenGL_Window_SDL;
	log_msg("graph opengl opengl_info sdl", "Using VideoAPI::SDL");
}
#endif


bool OpenGL_Engine::init(Config* _conf, Input *input)
{
	conf = _conf;
	if(conf == NULL)
	{
		log_msg("error graph conf res", "Cannot find renderer config file");
		return false;
	}

		string VideoAPI = conf->gets("VideoAPI");

		if(VideoAPI == "WinAPI")
		#if STEEL_OS == OS_WIN32
			UseWinAPI();
		#else
			error("graph opengl sdl opengl_info", "Cannot find VideoAPI::WinAPI");
		#endif

		if(VideoAPI == "SDL")
		#ifdef LIB_SDL
			UseSDL();
		#else
			error("graph opengl sdl opengl_info", "Cannot find VideoAPI::SDL");
		#endif

	#if STEEL_OS == OS_WIN32
		if(CreateOpenGL_Window == NULL)	UseWinAPI();
	#endif

	#ifdef LIB_SDL
		if(CreateOpenGL_Window == NULL)	UseSDL();
	#endif

		if(CreateOpenGL_Window == NULL)
		{
			error("graph opengl sdl opengl_info", "Cannot find VideoAPI");
			return false;
		}

/*	conf->setDefault("window.left", "10");
	conf->setDefault("window.top", "10");

	conf->setDefault("window.width", "800");
	conf->setDefault("window.height", "600");

	conf->setDefault("screen.depth", "32");
	conf->setDefault("screen.width", "800");
	conf->setDefault("screen.height", "600");

	conf->setDefault("fullscreen", "0");*/

	if (!(this->*CreateOpenGL_Window)(input))
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

	(this->*setCaptionOpenGL_Window)("Steel Engine");

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

	double cameraAspectRatio = conf->getd("window.width") / conf->getd("window.height");

	gluPerspective(conf->getd("camera.fov"), cameraAspectRatio, conf->getd("camera.min_dist"), conf->getd("camera.max_dist"));

    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z, 
			camera.center.x, camera.center.y, camera.center.z, 
			camera.up.x, camera.up.y, camera.up.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGL_Engine::prepare(GraphShadow *storage, steel::time globalTime, steel::time time, matrix34 matrix, GameObject *parent)
{
	info.curTime = globalTime;
	info.frameLength = time;
//	info.modificationTime = globalFrameNumber;
	info.cameraEye = camera.eye;
	info.cameraDirection = camera.center - camera.eye;

	storage->object->updateInformation(this->interfaceId, this);

	storage->setParent(parent);
	storage->cache();

/*	if(storage->childrenModificationTime < storage->object->getChildrenModificationTime())
	{
		storage->childrenModificationTime = storage->object->getChildrenModificationTime();

		StorageHash newChildrenId;

		int count = G(storage->object)->getGraphChildrenCount();
		for(int i = 0; i < count; i++) // add new + cache
		{
			GraphObject *child = G(storage->object)->getGraphChildren(i);
			uid id = child->getId();
			newChildrenId[id] = i;

			if(idHash.find(id) == idHash.end())
			{
				// add new object to storage
				makeStorageForObject(child);
				makeStorageForChildren(child);

				storage->children.push_back(id);
			}
			getStorage(child)->cache();
		}
		int size = storage->children.size();

		for(int i = 0; i < size; i++)
		{
			uid id = storage->children[i];
			if(newChildrenId.find(id) == newChildrenId.end())
			{
				int n = findSid(id);
				deleteStorageForChildren(n);
				deleteStorageForObject(n);
				storage->children[i] = storage->children.back();
				storage->children.pop_back();
				size--;
				i--;
			}
		}
	}

	if(storage->lights != NULL)
	for(Lights::const_iterator it = storage->lights->begin(); it != storage->lights->end(); it++)
	{
		lights.push_back(*it);
		lights.back().position = storage->position*lights.back().position;
	}
*/
/*	int count = G(storage->object)->getGraphChildrenCount();
	for(int i = 0; i < count; i++)
	{
		GraphObject *child = G(storage->object)->getGraphChildren(i);
		
		prepare(getStorage(child), globalTime, time);
	}*/
}


void OpenGL_Engine::makeStorageForChildren(GameObject *object)
{
/*	int count = G(object)->getGraphChildrenCount();
	for(int i = 0; i < count; i++)
	{
		GraphObject *child = G(object)->getGraphChildren(i);
		makeStorageForObject(child);
		makeStorageForChildren(child);
	}*/
}

void OpenGL_Engine::GraphShadow::fill(GameObject *object)
{
	Shadow::fill(object);
}

bool OpenGL_Engine::GraphShadow::cache(void)
{
	return  false;
}

void OpenGL_Engine::onResize(int width, int height)
{
	conf->setValued("window.width", width);
	conf->setValued("window.height", height);
	if(RepairOpenGL_Window)
		(this->*RepairOpenGL_Window)();
}
