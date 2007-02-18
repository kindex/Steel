/*id*********************************************************
	File: graph/opengl/opengl_engine.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
#include "../../common/containers.h"

#include "../../res/image/image.h"
#include "../../libs/opengl/libext.h"
#include "ext/normalisation_cube_map.h"
#include "../../res/res_main.h"

#include <algorithm>
#include <gl/glu.h>

using namespace std;

const TexCoords* OpenGL_Engine::GraphShadow::getTexCoords(const MaterialStd::TextureStd& texture)
{
	if (texture.texCoordsUnit < texCoords.size())
	{
		return texCoords[texture.texCoordsUnit];
	}
	else
	{
		if (!texCoords.empty())
		{
			return texCoords[0];
		}
	}
	return NULL;
}


void OpenGL_Engine::DrawFill_Material(OpenGL_Engine::GraphShadow &e, const Triangles *triangles, Material *material)
{
	if (material != NULL)
	{
		switch(material->getMaterialType())
		{
		case MATERIAL_STD:	
			if (DrawFill_MaterialStd != NULL)
			{
				if ((this->*DrawFill_MaterialStd)(e, *triangles, *static_cast<MaterialStd*>(material)))
				{
					return;
				}
			}
			break;
		}

		DrawFill_Material(e, triangles, material->reserve);
	}
}

OpenGL_Engine::OpenGL_Engine(): 
	BindTexture(NULL), 
	DrawFill_MaterialStd(NULL), 
	DrawTriangles(NULL),
	CleanupDrawTriangles(NULL),
	BindTexCoords(NULL),
	BindTexCoords3f(NULL),
	DrawWire(NULL),
	DrawLines(NULL),  
	DrawNormals(NULL),
	DrawVertexes(NULL),
	DrawAABB(NULL),
	
	windowInformation(NULL),

	CreateOpenGL_Window(NULL),
	RepairOpenGL_Window(NULL),
	DeleteOpenGL_Window(NULL),
	setCaptionOpenGL_Window(NULL),
	FlushOpenGL_Window(NULL),
	currentShadow(NULL) 
{}


void OpenGL_Engine::pushPosition(GraphShadow& e)
{
	if (e.positionKind == POSITION_SCREEN)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
	}
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	float m[16]; // TODO
	m[0] = e.realPosition.data.matrix.data.m[0][0];	m[1] = e.realPosition.data.matrix.data.m[1][0];	m[2] = e.realPosition.data.matrix.data.m[2][0];	m[3]  = 0;
	m[4] = e.realPosition.data.matrix.data.m[0][1];	m[5] = e.realPosition.data.matrix.data.m[1][1];	m[6] = e.realPosition.data.matrix.data.m[2][1];	m[7]  = 0;
	m[8] = e.realPosition.data.matrix.data.m[0][2];	m[9] = e.realPosition.data.matrix.data.m[1][2];	m[10]= e.realPosition.data.matrix.data.m[2][2];	m[11] = 0;
	m[12]= e.realPosition.data.vector.x;			m[13]= e.realPosition.data.vector.y;			m[14]= e.realPosition.data.vector.z;			m[15] = 1;
	glLoadMatrixf(m);
}

void OpenGL_Engine::popPosition(GraphShadow& e)
{
	glPopMatrix();
	if (e.positionKind == POSITION_SCREEN)
	{
		glPopMatrix();
	}
}






bool OpenGL_Engine::isVisible(AABB aabb)
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

void OpenGL_Engine::updateRealPosition(IN OUT GraphShadow* object)
{
	if (object->realPositionCalculated) return;

	if (object->positionKind == POSITION_GLOBAL || object->positionKind == POSITION_SCREEN || object->parent == NULL)
	{
		object->realPosition = object->position;
		object->realPositionCalculated = true;
	}
	else
	{
		GraphShadow* parent = getShadow(object->parent);
		updateRealPosition(parent);
		object->realPosition = object->position * parent->realPosition;
		object->realPositionCalculated = true;
	}
}

bool OpenGL_Engine::process(IN const ProcessInfo& _info)
{
	// TODO repair DC 

	info = _info;
	processCamera();

	total.vertexCount = 0;
	total.triangleCount = 0;
	total.objectCount = 0;

	int size = objects.size();

	for(int i=0; i < size; i++)
	{
		prepare(*getShadow(objects[i])); /* Update vertexes, faces, ights */
	}
	// update position
	
	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow* s = GS(*it);
		s->realPositionCalculated = false;
	}

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow* s = GS(*it);
		updateRealPosition(s);
	}

	for(map<uid, LightShadow*>::iterator jt = lights.begin(); jt != lights.end(); jt++)
	{
		jt->second->position = jt->second->light->position * jt->second->shadow->realPosition;
	}

	for EACH(ShadowPVector,  shadows, it)
	{
		GraphShadow &shadow = *GS(*it);
		shadow.lights.clear();
		if (!shadow.aabb.empty())
		{
			for(map<uid, LightShadow*>::iterator jt = lights.begin(); jt != lights.end(); jt++)
				if (shadow.isCrossingLight(jt->second))
				{
					shadow.lights.push_back(jt->second);
				}
		}
	}

	GLbitfield clear = 0;
	if(conf->geti("clearColor", 1))	clear |= GL_COLOR_BUFFER_BIT;
	if(conf->geti("clearDepth", 1))	clear |= GL_DEPTH_BUFFER_BIT;
	if(clear)
	{
		glClear(clear);
	}
	pvector<BlendingFaces> blendingFaces;
//	steel::vector<int> elementAlpha;
// В начале выводим только непрозрачные объекты
	for EACH(ShadowPVector, shadows, it)
	{
		FaceMaterialVector skippedFaces;
		process(*GS(*it), skippedFaces);

		for EACH(FaceMaterialVector, skippedFaces, jt)
		{
			blendingFaces.push_back(BlendingFaces(GS(*it), jt->material, jt->triangles));
		}
	}

// Потом прозрачные в порядке удалённости от камеры: вначале самые дальние

	if (conf->geti("drawAlpha")>0 && !blendingFaces.empty())
	{
		BlendingTriangleVector blendingTriangles;
		for EACH(pvector<BlendingFaces>, blendingFaces, it)
		{
			if (it->shadow->positionKind != POSITION_SCREEN)
			{
				for EACH(TriangleVector, it->triangles->data, jt)
				{
					BlendingTriangle newBlendingTriangle;
					newBlendingTriangle.vertex[0] = jt->a[0];
					newBlendingTriangle.vertex[1] = jt->a[1];
					newBlendingTriangle.vertex[2] = jt->a[2];
					v3 center = (it->shadow->realPosition * it->shadow->vertexes->data[jt->a[0]] + it->shadow->realPosition * it->shadow->vertexes->data[jt->a[1]] + it->shadow->realPosition * it->shadow->vertexes->data[jt->a[2]])/3.0;
					newBlendingTriangle.distance = (info.camera.getPosition() - center).getLength();
					newBlendingTriangle.material = it->material;
					newBlendingTriangle.shadow = it->shadow;
					blendingTriangles.push_back(newBlendingTriangle);
				}
			}
		}

		sort(blendingTriangles.begin(), blendingTriangles.end());

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);		
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		for EACH(BlendingTriangleVector, blendingTriangles, it)
		{
			Triangles triangles;
			triangles.data.push_back(Triangle(it->vertex[0], it->vertex[1], it->vertex[2]));
			triangles.changed = true;
			triangles.setId(0);
			DrawFill_Material(*it->shadow, &triangles, it->material);
		}

		glPopAttrib();
	}

	if(conf->geti("swapBuffers", 1))
	{
//		glFlush(); // TODO: flush in thread
		(this->*FlushOpenGL_Window)();
	}

	GLenum errorCode = glGetError();
	if(errorCode != 0)
	{
		log_msg("error opengl", string("OpenGL error #") + IntToStr(errorCode));
	}

	return true;
}

/*
Сердце Графического движка.
Отвечает за вывод графичесткого элемента.
*/

void OpenGL_Engine::process(GraphShadow& e, OUT FaceMaterialVector& skippedFaces)
{
	pushPosition(e);

	if (conf->getb("drawFace") && e.faceMaterials != NULL)
	{
		for EACH_CONST(FaceMaterialVector, *e.faceMaterials, it)
		{
			if (it->material->blend == TEXTURE_MODE_NONE)
			{
				DrawFill_Material(e, it->triangles, it->material);
			}
			else
			{
				skippedFaces.push_back(FaceMaterial(it->material, it->triangles));
			}
		}
	}

	if(conf->getb("drawWire") && e.faceMaterials != NULL && DrawWire != NULL)
	{
		for EACH_CONST(FaceMaterialVector, *e.faceMaterials, it)
		{
			(this->*DrawWire)(e, *it->triangles);
		}
	}

	if(conf->getb("drawLines") && DrawLines)
	{
		(this->*DrawLines)(e);
	}

	if(conf->getb("drawNormals") && DrawNormals)
	{
		(this->*DrawNormals)(e);
	}

	if(conf->getb("drawVertexes") && DrawVertexes)
	{
		(this->*DrawVertexes)(e);
	}

	if(conf->getb("drawAABB") && DrawAABB)
	{
		(this->*DrawAABB)(e);
	}
	popPosition(e);
}


#if STEEL_OS == OS_WIN32
void OpenGL_Engine::UseWinAPI()
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
void OpenGL_Engine::UseSDL()
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
	DrawFill_MaterialStd = &OpenGL_Engine::DrawFill_MaterialStd_OpenGL10;
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
			DrawFill_MaterialStd = &OpenGL_Engine::DrawFill_MaterialStd_OpenGL13;
	}

	if(version >= 15)
	{
		if(GL_EXTENSION_VBO)
		{
			DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL15;
			CleanupDrawTriangles = &OpenGL_Engine::CleanupDrawTriangles_OpenGL15;
			BindTexCoords = &OpenGL_Engine::BindTexCoords_OpenGL15;
			BindTexCoords3f = &OpenGL_Engine::BindTexCoords3f_OpenGL15;
		}
	}

	if(GL_EXTENSION_GLSL && conf->getb("use_glsl", true))
	{
		DrawFill_MaterialStd = &OpenGL_Engine::DrawFill_MaterialStd_OpenGL20;
		shaderStd.vertexShader = resText.add("material/" + conf->gets("std_shader", "std") + ".vert");
		shaderStd.fragmentShader = resText.add("material/" + conf->gets("std_shader", "std") + ".frag");
		maxLightsInShader = conf->geti("max_lights", 1);
	}
	textureMatrixLevel = 0;

	normalisationCubeMap	= generateNormalisationCubeMap();
	zeroNormal = resImage.add("zero");
	white = resImage.add("white");
	black = resImage.add("black");
	none = resImage.add("none");
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

	v3 cameraPosition = info.camera.getPosition();
	v3 cameraCenter = cameraPosition + info.camera.getDirection();
	v3 cameraUp = info.camera.getUpVector();

    gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			cameraCenter.x, cameraCenter.y, cameraCenter.z,
			cameraUp.x, cameraUp.y, cameraUp.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGL_Engine::prepare(GraphShadow& shadow, matrix34 matrix, GameObject* parent)
{
	currentShadow = &shadow;
	shadow.object->updateInformation(*this);

	shadow.setParent(parent);
	shadow.cache();

	for EACH(uidVector, shadow.children, it)
	{
		prepare(*getShadow(*it), matrix,  // TODO
					shadow.object);
	}

	if (!shadow.aabbCalculated)
	{
		shadow.calculateAABB();
	}

/*	if(shadow->childrenModificationTime < shadow->object->getChildrenModificationTime())
	{
		shadow->childrenModificationTime = shadow->object->getChildrenModificationTime();

		ShadowHash newChildrenId;

		int count = G(shadow->object)->getGraphChildrenCount();
		for(int i = 0; i < count; i++) // add new + cache
		{
			GraphObject *child = G(shadow->object)->getGraphChildren(i);
			uid id = child->getId();
			newChildrenId[id] = i;

			if(idHash.find(id) == idHash.end())
			{
				// add new object to shadow
				makeShadowForObject(child);
				makeShadowForChildren(child);

				shadow->children.push_back(id);
			}
			getShadow(child)->cache();
		}
		int size = shadow->children.size();

		for(int i = 0; i < size; i++)
		{
			uid id = shadow->children[i];
			if(newChildrenId.find(id) == newChildrenId.end())
			{
				int n = findSid(id);
				deleteShadowForChildren(n);
				deleteShadowForObject(n);
				shadow->children[i] = shadow->children.back();
				shadow->children.pop_back();
				size--;
				i--;
			}
		}
	}

*/
/*	int count = G(shadow->object)->getGraphChildrenCount();
	for(int i = 0; i < count; i++)
	{
		GraphObject *child = G(shadow->object)->getGraphChildren(i);
		
		prepare(getShadow(child), globalTime, time);
	}*/
}

void OpenGL_Engine::GraphShadow::fill(GameObject *object)
{
	Shadow::fill(object);
}

bool OpenGL_Engine::GraphShadow::cache()
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

OpenGL_Engine::LightShadow::LightShadow(): 
	light(NULL), 
	object(NULL)
{}

OpenGL_Engine::GraphShadow::GraphShadow(Engine *engine): 
	Shadow(engine), 
	faceMaterials(NULL), 
	vertexes(NULL), 
	normals(NULL), 
	lines(NULL), 
	position(ObjectPosition::getIdentity()), 
	textureCount(0), 
	aabbCalculated(false) 
{}

void OpenGL_Engine::GraphShadow::calculateAABB()
{
	aabb.clear();

	if (vertexes != NULL)
	{
		for EACH_CONST(v3Vector, vertexes->data, i)
		{
			aabb.merge(*i);
		}
	}
	aabbCalculated = true;
}

bool OpenGL_Engine::GraphShadow::isCrossingLight(const LightShadow* light)
{
	AABB realaabb = aabb;
	realaabb.mul(realPosition);
	return realaabb.isCrossingSphere(light->position, light->light->maxDistance);
}

void OpenGL_Engine::unbindTexCoords()
{
	if (textureMatrixLevel > 0)
	{
		glMatrixMode(GL_TEXTURE);	glLoadIdentity();
		textureMatrixLevel = 0;
	}
}
