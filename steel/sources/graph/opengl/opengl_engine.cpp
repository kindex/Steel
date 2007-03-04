/*id*********************************************************
	File: graph/opengl/opengl_engine.cpp
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2007
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
#include "../../math/plane.h"

#include <algorithm>
#include <gl/glu.h>

namespace opengl
{

using namespace std;

void OpenGL_Engine::collectInformationFromObjects()
{
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

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow &shadow = *GS(*it);
		shadow.lights.clear();
		if (!shadow.aabb.empty())
		{
			for(map<uid, LightShadow*>::iterator jt = lights.begin(); jt != lights.end(); jt++)
			{
				if (shadow.isCrossingLight(jt->second))
				{
					shadow.lights.push_back(jt->second);
				}
			}
		}
	}
}

bool OpenGL_Engine::process(IN const ProcessInfo& _info)
{
// TODO repair DC 
// ----------- Setup Variables ------------------------
	info = _info;
	processCamera();
	setupVariables();

// ------------- Clear Screen ------------
	GLbitfield clear = 0;
	if (conf->getb("clearColor", true))	clear |= GL_COLOR_BUFFER_BIT;
	if (conf->getb("clearDepth", true))	clear |= GL_DEPTH_BUFFER_BIT;
	if (flags.shadows)	clear |= GL_STENCIL_BUFFER_BIT;
	if (clear)
	{
		glClear(clear);
	}

// ------------ Draw Scene ---------------
	collectInformationFromObjects();

	if (flags.shadows)
	{
		renderShadows();
	}
	else
	{
		renderNormal();
	}

	renderDebug();

// ------------- Post draw ---------------

	if (conf->geti("swapBuffers", 1))
	{
//		glFlush(); // TODO: flush in thread
		(this->*FlushOpenGL_Window)();
	}
	GLenum errorCode = glGetError();
	if (errorCode != 0)
	{
		log_msg("error opengl", string("OpenGL error #") + IntToStr(errorCode));
	}
// -----------------------------------

	return true;
}

void OpenGL_Engine::setupShaderVariable(const std::string& key, const std::string& value, bool compile)
{
	if (currentShaderVariables[key] != value)
	{
		currentShaderVariables[key] = value;
		if (compile)
		{
			program = bindShader(flags.shaderStd, currentShaderVariables);
			if (program == NULL) // shader compile error
			{
				flags.glsl = false;
			}
		}
	}
}

void OpenGL_Engine::renderNormal()
{
	program = NULL;
	if (flags.glsl)
	{
		if (!flags.textures)
		{
			program = bindShader(flags.shaderNoTexture, StringDict());
		}
		else if (flags.useDebugShader)
		{
			program = bindShader(flags.shaderDebug, StringDict());
		}
		else// use_std_shader
		{
			currentShaderVariables.clear();
			currentShaderVariables["lighting"] = IntToStr(flags.lighting);
			currentShaderVariables["blending"] = "0";
		}
	}
	pvector<BlendingFaces> blendingFaces;
// В начале выводим только непрозрачные объекты

	for EACH(ShadowPVector, shadows, it)
	{
		FaceMaterialVector skippedFaces;
		drawObject(*GS(*it), skippedFaces);
		for EACH(FaceMaterialVector, skippedFaces, jt)
		{
			blendingFaces.push_back(BlendingFaces(GS(*it), jt->material, jt->faces));
		}
	}
// Потом прозрачные в порядке удалённости от камеры: вначале самые дальние
	if (flags.textures && !flags.useDebugShader)
	{
		setupShaderVariable("blending", "1");
	}

	if (flags.blend && flags.transparent && !blendingFaces.empty())
	{
		BlendingTriangleVector blendingTriangles;
		for EACH(pvector<BlendingFaces>, blendingFaces, it)
		{
			if (it->shadow->positionKind != POSITION_SCREEN)
			{
				for EACH(TriangleVector, it->faces->triangles, jt)
				{
					BlendingTriangle newBlendingTriangle;
					newBlendingTriangle.vetexCount = 3;
					newBlendingTriangle.vertex[0] = jt->a[0];
					newBlendingTriangle.vertex[1] = jt->a[1];
					newBlendingTriangle.vertex[2] = jt->a[2];
					v3 center = (it->shadow->realPosition * it->shadow->vertexes->at(jt->a[0]) + it->shadow->realPosition * it->shadow->vertexes->at(jt->a[1]) + it->shadow->realPosition * it->shadow->vertexes->at(jt->a[2]))/3.0;
					newBlendingTriangle.distance = (info.camera.getPosition() - center).getLength();
					newBlendingTriangle.material = it->material;
					newBlendingTriangle.shadow = it->shadow;
					blendingTriangles.push_back(newBlendingTriangle);
				}
				for EACH(QuadVector, it->faces->quads, jt)
				{
					BlendingTriangle newBlendingTriangle;
					newBlendingTriangle.vetexCount = 4;
					newBlendingTriangle.vertex[0] = jt->a[0];
					newBlendingTriangle.vertex[1] = jt->a[1];
					newBlendingTriangle.vertex[2] = jt->a[2];
					newBlendingTriangle.vertex[3] = jt->a[3];
					v3 center = (it->shadow->realPosition * it->shadow->vertexes->at(jt->a[0]) + it->shadow->realPosition * it->shadow->vertexes->at(jt->a[1]) + it->shadow->realPosition * it->shadow->vertexes->at(jt->a[2]) + it->shadow->realPosition * it->shadow->vertexes->at(jt->a[3]))/4.0;
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
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		BlendingTriangle* prev = NULL;
		Faces faces;
		faces.triangles.changed = true;
		faces.triangles.setId(0);
		faces.quads.changed = true;
		faces.quads.setId(0);

		for EACH(BlendingTriangleVector, blendingTriangles, it)
		{
			if (prev != NULL && (
					prev->shadow != it->shadow || 
					it->vetexCount == 3 && !faces.quads.empty() ||
					it->vetexCount == 4 && !faces.triangles.empty()
				))
			{
				(this->*DrawFill_MaterialStd)(*prev->shadow, faces, *static_cast<MaterialStd*>(prev->material));
				faces.quads.clear();
				faces.triangles.clear();
			}
			prev = &(*it);
			if (it->vetexCount == 3) // 3 - triangle
			{
				faces.triangles.push_back(Triangle(it->vertex[0], it->vertex[1], it->vertex[2]));
			}
			else // 4 - quads
			{
				faces.quads.push_back(Quad(it->vertex[0], it->vertex[1], it->vertex[2], it->vertex[3]));
			}
		}
		(this->*DrawFill_MaterialStd)(*prev->shadow, faces, *static_cast<MaterialStd*>(prev->material));

		glPopAttrib();
	}
	if (program != NULL)
	{
		program->unbind();
	}
}

void OpenGL_Engine::renderShadows()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//program = bindShader("material/no_texture", StringDict());
//	if (program == NULL)return;
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
		pushPosition(e);
		if (e.faceMaterials != NULL && !e.lights.empty())
		{
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
			{
				if (faces->material->blend == 0)
				{
					(this->*DrawTriangles)(e, *faces->faces, NULL);
				}
			}
		}
		popPosition(e);
	}
//	program->unbind();

	program = bindShader("material/shadow", StringDict());
	if (program == NULL) return;

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0, 0xffffffff);

	for (int step = 0; step < 2; step++)
	{
		if (step == 0)
		{
			glFrontFace(GL_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		}
		else
		{
			glFrontFace(GL_CW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		}

		for EACH(ShadowPVector, shadows, it)
		{
			GraphShadow& e = *GS(*it);
		
			pushPosition(e);
			if (e.faceMaterials != NULL && !e.lights.empty())
			{
				program->setUniformVector("lightPos", e.lights[0]->position);
				for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
				{
					if (e.vertexes != NULL && !(faces->faces->triangles.empty()&& faces->faces->quads.empty()) && !e.vertexes->empty())// если есть полигоны и вершины
					{
						for EACH_CONST(TriangleVector, faces->faces->triangles, it)
						{
							Plane a;
							a.base = e.vertexes->at( it->a[0] );
							a.a = e.vertexes->at( it->a[1] ) - a.base;
							a.b = e.vertexes->at( it->a[2] ) - a.base;

							if (byRightSide(e.lights[0]->position, a))
							{
								glBegin(GL_QUADS);
								for (int i = 0; i < 3; i++)
								{
									glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at( it->a[0 + i] ).getfv());
									glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at( it->a[(1 + i)%3] ).getfv());
									glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at( it->a[(1 + i)%3] ).getfv());
									glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at( it->a[0 + i] ).getfv());
								}
								glEnd();
							}
						}


						for EACH_CONST(QuadVector, faces->faces->quads, it)
						{
							Plane a;
							a.base = e.vertexes->at( it->a[0] );
							a.a = e.vertexes->at( it->a[1] ) - a.base;
							a.b = e.vertexes->at( it->a[2] ) - a.base;

							if (byRightSide(e.lights[0]->position, a))
							{
								glBegin(GL_QUADS);
								for (int i = 0; i < 4; i++)
								{
									glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at( it->a[0 + i] ).getfv());
									glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at( it->a[(1 + i)%4] ).getfv());
									glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at( it->a[(1 + i)%4] ).getfv());
									glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at( it->a[0 + i] ).getfv());
								}
								glEnd();
							}
						}
					}
				}
			}
			popPosition(e);
		}
	}
	program->unbind();

	glEnable(GL_STENCIL_TEST);

	glFrontFace(GL_CCW);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 0, 0xffffffff);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//glBegin(GL_QUADS);
	//	glVertex2f(-0.9, 0.9);
	//	glVertex2f(-0.9,-0.9);
	//	glVertex2f( 0.9,-0.9);
	//	glVertex2f( 0.8, 0.8);
	//glEnd();
	//glPopMatrix();

	glDepthMask(GL_FALSE);
	renderNormal();

	glPopAttrib();
}

/*
Сердце Графического движка.
Отвечает за вывод графичесткого элемента.
*/

void OpenGL_Engine::drawObject(GraphShadow& e, OUT FaceMaterialVector& skippedFaces)
{
	pushPosition(e);

	if (flags.drawFace && e.faceMaterials != NULL)
	{
		for EACH_CONST(FaceMaterialVector, *e.faceMaterials, it)
		{
			if (it->material != NULL)
			{
				if (it->material->blend == TEXTURE_MODE_NONE || (!flags.blend && flags.transparent))
				{
					(this->*DrawFill_MaterialStd)(e, *it->faces, *static_cast<MaterialStd*>(it->material));
				}
				else if (flags.transparent)
				{
					skippedFaces.push_back(FaceMaterial(it->material, it->faces));
				}
			}
		}
	}
	popPosition(e);
}

void OpenGL_Engine::renderDebug()
{
	if (flags.drawWire)
	{
		for EACH(ShadowPVector, shadows, e)
		{
			if (GS(*e)->faceMaterials != NULL)
			{
				pushPosition(*GS(*e));
				for EACH_CONST(FaceMaterialVector, *GS(*e)->faceMaterials, it)
				{
					(this->*DrawWire)(*GS(*e), *it->faces);
				}
				popPosition(*GS(*e));
			}
		}
	}
#define drawAllShadowsDebug(flag, function) \
	if (flag) \
	{ \
		for EACH(ShadowPVector, shadows, e) \
		{ \
			if (GS(*e)->faceMaterials != NULL) \
			{ \
				pushPosition(*GS(*e)); \
				(function)(*GS(*e)); \
				popPosition(*GS(*e)); \
			} \
		} \
	}

	drawAllShadowsDebug(flags.drawLines, this->*DrawLines);
	drawAllShadowsDebug(flags.drawNormals, this->*DrawNormals);
	drawAllShadowsDebug(flags.drawVertexes, this->*DrawVertexes);
	drawAllShadowsDebug(flags.drawAABB, this->*DrawAABB);

#undef drawAllShadowsDebug
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
		if (CreateOpenGL_Window == NULL)	UseWinAPI();
	#endif

	#ifdef LIB_SDL
		if (CreateOpenGL_Window == NULL)	UseSDL();
	#endif

	if (CreateOpenGL_Window == NULL)
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


	log_msg("graph opengl opengl_info", "Renderer: " + (string)(const char*)glGetString(GL_RENDERER));
	log_msg("graph opengl opengl_info", "Version: " + (string)(const char*)glGetString(GL_VERSION));
	log_msg("graph opengl opengl_info", "Vendor: " + (string)(const char*)glGetString(GL_VENDOR));

	string openglVersions = (const char *)glGetString(GL_VERSION);
	int openglVersioni;
	
	if(openglVersions.length() >= 3)
	{
		openglVersioni = (openglVersions[0] - '0')*10 + (openglVersions[2] - '0');
	}
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

	if (version >= 11)
	{
		BindTexture = &OpenGL_Engine::BindTexture_OpenGL11;
		DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL11;
		DrawWire = &OpenGL_Engine::DrawWire_OpenGL11;
		DrawLines = &OpenGL_Engine::DrawLines_OpenGL11;
		BindTexCoords = &OpenGL_Engine::BindTexCoords_OpenGL11;
		BindTexCoords3f = &OpenGL_Engine::BindTexCoords3f_OpenGL11;
	}

	if (version >= 13)
	{
		if (GL_EXTENSION_MULTITEXTURE)
		{
			DrawFill_MaterialStd = &OpenGL_Engine::DrawFill_MaterialStd_OpenGL13;
		}
	}

	if (version >= 15)
	{
		if (GL_EXTENSION_VBO && conf->getb("use_vbo", true))
		{
			DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL15;
			CleanupDrawTriangles = &OpenGL_Engine::CleanupDrawTriangles_OpenGL15;
			BindTexCoords = &OpenGL_Engine::BindTexCoords_OpenGL15;
			BindTexCoords3f = &OpenGL_Engine::BindTexCoords3f_OpenGL15;
		}
	}
	setupVariables();
	if (version >= 20 && GL_EXTENSION_GLSL && conf->getb("use_glsl", true))
	{
		flags.maxLightsInShader = conf->geti("max_lights", 4);
		StringDict parameters;
		parameters["lighting"] = IntToStr(flags.lighting);
		parameters["lighcount"] = IntToStr(flags.maxLightsInShader);
		parameters["blending"] = "0";
		parameters["reflecting"] = "1";
		if (loadShader(flags.shaderStd, parameters))
		{
			flags.glsl = true;
			log_msg("info opengl glsl", "Your video driver supports GLSL. Test shader compiled succesefully. Max light count is set to " + IntToStr(flags.maxLightsInShader));
		}
		else
		{
			flags.maxLightsInShader--;
			while (flags.maxLightsInShader > 0)
			{
				parameters["lighcount"] = IntToStr(flags.maxLightsInShader);
				if (loadShader(flags.shaderStd, parameters))
				{
					break;
				}
				flags.maxLightsInShader--;
			}
			if (flags.maxLightsInShader <= 0)
			{
				log_msg("warning opengl glsl", "Your video driver supports GLSL, but it cannot compile default shader. Swithing to mutitexture renderer (OepnGL 1.3)");
			}
			else
			{
				log_msg("warning opengl glsl", "Your video driver supports GLSL, but max light count will be limited to " + IntToStr(flags.maxLightsInShader));
				flags.glsl = true;
			}
		}
		if (flags.glsl)
		{
			DrawFill_MaterialStd = &OpenGL_Engine::DrawFill_MaterialStd_OpenGL20;
		}
	}
	else
	{
		flags.glsl = false;
	}
	textureMatrixLevel = 0;

	normalisationCubeMap = generateNormalisationCubeMap();
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

void OpenGL_Engine::onResize(int width, int height)
{
	conf->setValued("window.width", width);
	conf->setValued("window.height", height);
	if(RepairOpenGL_Window)
		(this->*RepairOpenGL_Window)();
}

GraphShadow::GraphShadow(Engine *engine): 
	Shadow(engine), 
	faceMaterials(NULL), 
	vertexes(NULL), 
	normals(NULL), 
	lines(NULL), 
	position(ObjectPosition::getIdentity()), 
	textureCount(0), 
	aabbCalculated(false) 
{}

void OpenGL_Engine::unbindTexCoords()
{
	if (textureMatrixLevel > 0)
	{
		glMatrixMode(GL_TEXTURE);	glLoadIdentity();
		textureMatrixLevel = 0;
	}
}

bool OpenGL_Engine::setCaption(const std::string& caption)
{
	return (this->*setCaptionOpenGL_Window)(caption);
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
	currentShadow(NULL),
	program(NULL)
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
	// TODO:
/*
	matrix34 proj; 
	glGetFloatv(GL_PROJECTION_MATRIX, proj.a);
	box.mul(proj);

	return box.intersect(aabb(v3(-1.1,-1.1,0), v3(1.1,1.1,2)));
*/
	return true;
}

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

void OpenGL_Engine::setupVariables()
{
	total.vertexCount = 0;
	total.triangleCount = 0;
	total.batchCount = 0;

	flags.lighting = conf->getb("lighting", true);
	flags.textures = conf->getb("textures", true);
	flags.drawFace = conf->getb("drawFace", true);
	flags.blend = conf->getb("blend", true);
	flags.transparent = conf->getb("transparent", true);
	flags.bump = conf->getb("bump", true);
	flags.shadows = conf->getb("shadows", true);

	flags.drawWire = conf->getb("drawWire", false) && DrawWire != NULL;
	flags.drawLines = conf->getb("drawLines", false) && DrawLines != NULL;
	flags.drawNormals = conf->getb("drawNormals", false) && DrawNormals != NULL;
	flags.drawVertexes = conf->getb("drawVertexes", false) && DrawVertexes != NULL;
	flags.drawAABB = conf->getb("drawAABB", false) && DrawAABB;
	flags.useDebugShader = conf->getb("use_debug_shader", false);
	flags.debugShaderMode = conf->geti("debug_shader_mode", 2);

	flags.shaderStd = "material/" + conf->gets("std_shader", "std");
	flags.shaderDebug = "material/" + conf->gets("debug_shader", "debug");
	flags.shaderNoTexture = "material/" + conf->gets("no_texture_shader", "no_texture");
}


} // namespace opengl
