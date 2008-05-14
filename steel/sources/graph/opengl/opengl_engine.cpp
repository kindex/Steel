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
#include "opengl_glsl.h"
#include "../../common/utils.h"
#include "../../common/logger.h"
#include "../../res/image/image.h"
#include "ext/libext.h"
#include "ext/normalisation_cube_map.h"
#include "../../res/res_main.h"
#include "../../math/plane.h"
#include "../../math/sprite.h"
#include "../../engine/game_object.h"
#include "../material.h"
#include <gl/glu.h>
#undef min
#undef DrawText
#include <algorithm>

namespace opengl
{

using namespace std;

void OpenGL_Engine::collectInformationFromObjects()
{
	int size = objects.size();

	for(int i = 0; i < size; i++)
	{
		prepare(*getShadow(objects[i])); /* Update vertexes, faces, lights */
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

	for EACH(LightMap, lights, jt)
	{
		jt->second->position = jt->second->light->position * jt->second->shadow->realPosition;
		total.lightCount++;
	}

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow &shadow = *GS(*it);
		shadow.lights.clear();
		if (!shadow.aabb.empty())
		{
			for(map<uid, LightShadow*>::iterator jt = lights.begin(); jt != lights.end(); jt++)
			{
				if (jt->second->light->cubeMap != NULL)
				{
					if (shadow.isCrossingLight(jt->second))
					{
						shadow.lights.push_back(jt->second);
					}
				}
			}
			for(map<uid, LightShadow*>::iterator jt = lights.begin(); jt != lights.end(); jt++)
			{
				if (jt->second->light->cubeMap == NULL)
				{
					if (shadow.isCrossingLight(jt->second))
					{
						shadow.lights.push_back(jt->second);
					}
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
	if (conf->getb("swapBuffers", true))
	{
		(this->*FlushOpenGL_Window)(); // TODO: flush in thread
	}
    if (conf->getb("singleBuffer", false))
    {
		(this->*FlushOpenGL_Window)();
    }

	GLbitfield clear = 0;
	if (conf->getb("clearColor", true))	clear |= GL_COLOR_BUFFER_BIT;
	if (conf->getb("clearDepth", true))	clear |= GL_DEPTH_BUFFER_BIT;
	if (clear)
	{
		glClear(clear);
	}

// ------------ Draw Scene ---------------
	collectInformationFromObjects();

	render();
	renderDebug();

// ------------- Post draw ---------------

	GLenum errorCode = glGetError();
	if (errorCode != 0)
	{
        log_msg("error opengl", string("OpenGL error #") + IntToStr(errorCode) + ": " + (const char*)gluErrorString(errorCode));
	}
// -----------------------------------

	return true;
}

void OpenGL_Engine::renderText()
{
    if (font == NULL || DrawText == NULL)
    {
        return;
    }

    for EACH(ShadowPVector, shadows, shadow)
	{
		GraphShadow& e = *GS(*shadow);
        if (!e.text.empty())
        {
            (this->*DrawText)(e.realPosition, e.text);
        }
	}
}

void OpenGL_Engine::renderTransparent()
{
// К этому моменту все непрозрачные объекты уже выведены
	if (!flags.blending || flags.current.transparent)
	{
		return;
	}
	flags.current.transparent = true;
	pvector<BlendingFaces> blendingFaces;

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
		if (e.faceMaterials != NULL)
		{
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, jt)
			{
				if (jt->material != NULL && jt->material->blend)
				{
					blendingFaces.push_back(BlendingFaces(&e, jt->material, jt->faces));
				}
			}
		}
	}
// выводим прозрачные в порядке удалённости от камеры: вначале самые дальние

	if (flags.blending && flags.current.transparent && !blendingFaces.empty())
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
		if (prev != NULL)
		{
			(this->*DrawFill_MaterialStd)(*prev->shadow, faces, *static_cast<MaterialStd*>(prev->material));
		}

		glPopAttrib();
	}
	if (program != NULL)
	{
		program->unbind();
	}
}


struct ShadowFace
{
	int e[4];
	bool visible;
};

void OpenGL_Engine::renderCatchShadows()
{
	flags.current.transparent = false;
	flags.current.simpleLighting = true;
	flags.current.shadowLighting = false;
	flags.current.onlyLight = 0;
	flags.current.shadowDebug = 0;

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
		if (e.faceMaterials != NULL)
		{
            pushPosition(e.realPosition, e.positionKind);
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
			{
				if (faces->material != NULL && faces->material->blend == 0 && faces->material->catchShadows)
				{
					(this->*DrawFill_MaterialStd)(e, *faces->faces, *faces->material);
				}
			}
            popPosition(e.positionKind);
		}
	}
}

void OpenGL_Engine::prepareShadowEdges()
{
	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
		if (e.faceMaterials != NULL)
		{
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
			{
				if (faces->material->dropShadows)
				{
					e.calculateEgdes();
					break;
				}
			}
		}
	}
}

void OpenGL_Engine::renderNoShadows()
{
	flags.current.transparent = false;
	flags.current.simpleLighting = true;
	flags.current.shadowLighting = true;
	flags.current.onlyLight = 0;
	flags.current.shadowDebug = 0;

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
		if (e.faceMaterials != NULL)
		{
            pushPosition(e.realPosition, e.positionKind);
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
			{
				if (faces->material != NULL && faces->material->blend == 0 && (!faces->material->catchShadows || !flags.shadows))
				{
					(this->*DrawFill_MaterialStd)(e, *faces->faces, *faces->material);
				}
			}
            popPosition(e.positionKind);
		}
	}
}


void OpenGL_Engine::render()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	if (flags.shadows)
	{
		renderCatchShadows(); // step 1
		prepareShadowEdges(); // step 2

		for EACH(LightMap, lights, light)
		{
			if (light->second->light->castShadows)
			{
				castShadow(*light->second); // step 2
			}
		}
	}
    if (flags.drawFace)
    {
	    renderNoShadows();
    }
    if (program != NULL)
    {
	    program->unbind();
    }
	glPopAttrib();
    renderText();

	renderTransparent();
	renderFlares();
}

void OpenGL_Engine::renderFlares()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for EACH(LightMap, lights, lightIt)
	{
		Light& light = *lightIt->second->light;
		if (light.flare != NULL)
		{
			if (rayTrace(Line(info.camera.getPosition(), lightIt->second->position - info.camera.getPosition()), false) > 0)
			{
				continue;
			}

			v3 vertex1;
			v3 vertex2;
			v3 vertex3;
			v3 vertex4;
			v3 normal;
            v3 dx;
            v3 dy;
			calculateSprite(info.camera,
						lightIt->second->position,
                        POSITION_GLOBAL,
						v2(light.flareSize, light.flareSize),
						SPRITE_ALIGN_SCREEN,
						v3(),
						vertex1,
						vertex2,
						vertex3,
						vertex4,
						normal,
                        dx,
                        dy);

			(this->*BindTexture)(*light.flare, true);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0, 0.0); glVertex3fv(vertex1.getfv());
				glTexCoord2f(1.0, 1.0); glVertex3fv(vertex2.getfv());
				glTexCoord2f(0.0, 1.0); glVertex3fv(vertex3.getfv());
				glTexCoord2f(0.0, 0.0); glVertex3fv(vertex4.getfv());
			glEnd();
		}
	}
	glPopAttrib();
}

void OpenGL_Engine::castShadow(const LightShadow& light)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	Line lineSegment(info.camera.getPosition(), light.position - info.camera.getPosition());
	size_t inShadowVolume = rayTrace(lineSegment, true);

	float maxDistance = light.light->maxDistance;

	glClear(GL_STENCIL_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0, 0xffffffff);

	program = bindShader("material/shadow", StringDict());
	if (program != NULL)
	{
		program->setUniformVector("lightPos", light.position);
		program->setUniformFloat("maxDistance", maxDistance);
	}

	for (int step = 0; step < 2; step++)
	{
		if (step == 0)
		{
			glFrontFace(inShadowVolume > 0 ? GL_CW : GL_CCW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		}
		else
		{
			glFrontFace(inShadowVolume > 0 ? GL_CCW : GL_CW);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		}

		for EACH(ShadowPVector, shadows, it)
		{
			GraphShadow& e = *GS(*it);
			if (e.faceMaterials != NULL && e.vertexes != NULL && !e.vertexes->empty())
			{
				if (flags.glsl)
				{
                    pushPosition(e.realPosition, e.positionKind);
				}
				int faceNumber = 0;
				for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
				{
					if (!(faces->faces->triangles.empty() &&  faces->faces->quads.empty()) && faces->material->dropShadows)
					{
						svector<ShadowFace> triangles(faces->faces->triangles.size());
						svector<svector<int> > vertexes(e.vertexes->size());

						int i = 0;
						for EACH_CONST(TriangleVector, faces->faces->triangles, it)
						{
							Plane a;
							a.base = e.realPosition*e.vertexes->at(it->a[0]);
							a.a = e.realPosition*e.vertexes->at(it->a[1]) - a.base;
							a.b = e.realPosition*e.vertexes->at(it->a[2]) - a.base;

							triangles[i++].visible = byRightSide(light.position, a);
						}

						for EACH(EdgeVector, e.edges[faceNumber], edge)
						{
							if (
								edge->faceCount == 1 && triangles[edge->faces[0]].visible || 
								edge->faceCount == 2 && triangles[edge->faces[0]].visible != triangles[edge->faces[1]].visible
								)
							{
								int vert1;
								int vert2;
								if (triangles[edge->faces[0]].visible)
								{
									vert1 = edge->vertex[0];
									vert2 = edge->vertex[1];
								}
								else
								{
									vert1 = edge->vertex[1];
									vert2 = edge->vertex[0];
								}

								if (flags.current.shadowDebug)
								{
									glPushAttrib(GL_ALL_ATTRIB_BITS);
									glDisable(GL_STENCIL_TEST);
									glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
									glDepthFunc(GL_ALWAYS);
									glColor3f(1.0, 0, 0);
									glLineWidth(2);
									glDepthMask(GL_TRUE);
									glBegin(GL_LINE_LOOP);
										glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at(vert1).getfv());
										glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at(vert2).getfv());
										glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at(vert2).getfv());
										glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at(vert1).getfv());
									glEnd();
									glPopAttrib();
								}

								if (flags.glsl)
								{
									glBegin(GL_QUADS);
										glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at(vert1).getfv());
										glTexCoord1f(1.0f); glVertex3fv(e.vertexes->at(vert2).getfv());
										glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at(vert2).getfv());
										glTexCoord1f(0.0f); glVertex3fv(e.vertexes->at(vert1).getfv());
									glEnd();
								}
								else
								{
									v3 vert1g = e.realPosition*e.vertexes->at(vert1);
									v3 vert2g = e.realPosition*e.vertexes->at(vert2);
									v3 vert1s = vert1g + (vert1g - light.position)*maxDistance;
									v3 vert2s = vert2g + (vert2g - light.position)*maxDistance;

									glBegin(GL_QUADS);
										glVertex3fv(vert1s.getfv());
										glVertex3fv(vert2s.getfv());
										glVertex3fv(vert2g.getfv());
										glVertex3fv(vert1g.getfv());
									glEnd();
								}
							}
						}
					}
					faceNumber++;
				}
				if (flags.glsl)
				{
					popPosition(e.positionKind);
				}
			}
		}
	}
	if (program != NULL)
	{
		program->unbind();
	}
	glPopAttrib();

	flags.current.simpleLighting = false;
	flags.current.shadowLighting = true;
	flags.current.onlyLight = light.light->id;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_STENCIL_TEST);

	glFrontFace(GL_CCW);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, inShadowVolume > 0? inShadowVolume : 0, 0xffffffff);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
        pushPosition(e.realPosition, e.positionKind);
		if (flags.drawFace && e.faceMaterials != NULL)
		{
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, it)
			{
				if (it->material != NULL && it->material->blend == TEXTURE_MODE_NONE)
				{
					(this->*DrawFill_MaterialStd)(e, *it->faces, *static_cast<MaterialStd*>(it->material));
				}
			}
		}
		popPosition(e.positionKind);
	}
	glPopAttrib();
}


/*
Сердце Графического движка.
Отвечает за вывод графичесткого элемента.
*/

void OpenGL_Engine::drawObject(GraphShadow& e, OUT FaceMaterialVector& skippedFaces)
{
	pushPosition(e.realPosition, e.positionKind);
	if (flags.drawFace && e.faceMaterials != NULL)
	{
		for EACH_CONST(FaceMaterialVector, *e.faceMaterials, it)
		{
			if (it->material != NULL)
			{
				if (it->material->blend == TEXTURE_MODE_NONE || (!flags.blending && flags.current.transparent))
				{
					(this->*DrawFill_MaterialStd)(e, *it->faces, *static_cast<MaterialStd*>(it->material));
				}
				else if (flags.current.transparent)
				{
					skippedFaces.push_back(FaceMaterial(it->material, it->faces));
				}
			}
		}
	}
	popPosition(e.positionKind);
}

void OpenGL_Engine::renderDebug()
{
	if (flags.drawWire)
	{
		for EACH(ShadowPVector, shadows, e)
		{
			if (GS(*e)->faceMaterials != NULL)
			{
                pushPosition(GS(*e)->realPosition, GS(*e)->positionKind);
				for EACH_CONST(FaceMaterialVector, *GS(*e)->faceMaterials, it)
				{
					(this->*DrawWire)(*GS(*e), *it->faces);
				}
                popPosition(GS(*e)->positionKind);
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
				pushPosition(GS(*e)->realPosition, GS(*e)->positionKind); \
				(function)(*GS(*e)); \
				popPosition(GS(*e)->positionKind); \
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

    if (conf->getb("clearDepth"))
    {
	    glDepthFunc(GL_LESS);
    }
    else
    {
        glDepthFunc(GL_ALWAYS);
    }

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

	// if version is not set in config, then autodetect it
	int version = std::min(conf->geti("OpenGL_Version", 20), openglVersioni);

	log_msg("graph opengl opengl_info", "Using OpenGL renderer version: " + IntToStr(version));

	BindTexture = &OpenGL_Engine::BindTexture_OpenGL10;
	DrawFill_MaterialStd = &OpenGL_Engine::DrawFill_MaterialStd_OpenGL10;
	DrawTriangles = &OpenGL_Engine::DrawTriangles_OpenGL10;
	DrawWire = &OpenGL_Engine::DrawWire_OpenGL10;
	DrawLines = &OpenGL_Engine::DrawLines_OpenGL10;
	DrawNormals = &OpenGL_Engine::DrawNormals_OpenGL10;
	DrawVertexes = &OpenGL_Engine::DrawVertexes_OpenGL10;
	DrawAABB = &OpenGL_Engine::DrawAABB_OpenGL10;
	DrawText = &OpenGL_Engine::DrawText_OpenGL10;

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
		parameters["lightcount"] = IntToStr(flags.maxLightsInShader);
		parameters["targetlightcount"] = "0";
		parameters["blending"] = "0";
		parameters["mirror"] = "1";
		parameters["sky"] = "0";
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
				parameters["lightcount"] = IntToStr(flags.maxLightsInShader);
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
        if (flags.glsl && flags.lighting)
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
	font = resImage.add("font");
	if (zeroNormal == NULL)
	{
		log_msg("error graph res", "Zero normal map was not found");
		return false;
	}
	if (font == NULL)
	{
		log_msg("error graph res", "Font texture was not found");
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

	double cameraAspectRatio = conf->getd("window.width") / conf->getd("window.height");

	gluPerspective(conf->getd("camera.fov"), cameraAspectRatio, conf->getd("camera.min_dist"), conf->getd("camera.max_dist"));

	v3 cameraPosition = info.camera.getPosition();
	v3 cameraCenter = cameraPosition + info.camera.getDirection();
	v3 cameraUp = info.camera.getUpVector();

    gluLookAt(	cameraPosition.x,	cameraPosition.y,	cameraPosition.z,
				cameraCenter.x,		cameraCenter.y,		cameraCenter.z,
				cameraUp.x,			cameraUp.y,			cameraUp.z);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGL_Engine::prepare(GraphShadow& shadow, matrix34 matrix, GameObject* parent)
{
	currentShadow = &shadow;
	shadow.object->updateInformation(*this, INTERFACE_GRAPH);

	shadow.setParent(parent);

	for EACH(uidVector, shadow.children, it)
	{
		prepare(*getShadow(*it), matrix, shadow.object);
	}

	if (!shadow.aabbCalculated)
	{
		shadow.calculateAABB();
	}
}

void OpenGL_Engine::onResize(int width, int height)
{
	conf->setValued("window.width", width);
	conf->setValued("window.height", height);
	if (RepairOpenGL_Window)
    {
		(this->*RepairOpenGL_Window)();
    }
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
		glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);

        //glPopMatrix(); // OpenGL error
		textureMatrixLevel--;
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
    DrawText(NULL),
	
	windowInformation(NULL),

	CreateOpenGL_Window(NULL),
	RepairOpenGL_Window(NULL),
	DeleteOpenGL_Window(NULL),
	setCaptionOpenGL_Window(NULL),
	FlushOpenGL_Window(NULL),
	currentShadow(NULL),
	program(NULL),

	zeroNormal(NULL),
	black(NULL),
	white(NULL),
	none(NULL),
	font(NULL)
{}


void OpenGL_Engine::pushPosition(const ObjectPosition& position, PositionKind positionKind)
{
	if (positionKind == POSITION_SCREEN)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
	}
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

	float m[16]; // TODO
	m[0] = position.data.matrix.data.m[0][0];	m[1] = position.data.matrix.data.m[1][0];	m[2] = position.data.matrix.data.m[2][0];	m[3]  = 0;
	m[4] = position.data.matrix.data.m[0][1];	m[5] = position.data.matrix.data.m[1][1];	m[6] = position.data.matrix.data.m[2][1];	m[7]  = 0;
	m[8] = position.data.matrix.data.m[0][2];	m[9] = position.data.matrix.data.m[1][2];	m[10]= position.data.matrix.data.m[2][2];	m[11] = 0;
	m[12]= position.data.vector.x;			    m[13]= position.data.vector.y;			    m[14]= position.data.vector.z;			    m[15] = 1;
	glLoadMatrixf(m);
/*
		     (m[0]   m[4]   m[8]    m[12]	)	 ( v[0]	)
		     |m[1]   m[5]   m[9]	m[13]	|	 | v[1]	|
	  M(v) = |m[2]   m[6]   m[10]   m[14]	| x	 | v[2]	|
		     (m[3]   m[7]   m[11]   m[15]	)	 ( v[3]	)

( 1    0	 0    x	  )
| 0    1	 0    y	  |
| 0    0	 1    z	  |
( 0    0	 0    1	  )
*/
}

void OpenGL_Engine::popPosition(PositionKind positionKind)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	if (positionKind == POSITION_SCREEN)
	{
		glMatrixMode(GL_PROJECTION);
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
	if (object->realPositionCalculated)
    {
        return;
    }

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
	total.faceCount = 0;
	total.batchCount = 0;
	total.lightCount = 0;

	flags.lighting = conf->getb("lighting", true);
	flags.textures = conf->getb("textures", true);
	flags.drawFace = conf->getb("drawFace", true);
	flags.blending = conf->getb("blending", true);
//	flags.transparent = conf->getb("transparent", true);
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

size_t OpenGL_Engine::rayTrace(const Line& lineSegment, bool shadowed)
{
	size_t result = 0;
	for EACH(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);
		if (e.faceMaterials != NULL)
		{
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
			{
				if (shadowed <= faces->material->dropShadows)
				{
					for EACH(TriangleVector, faces->faces->triangles, it)
					{
						Plane triangle;
						triangle.base = e.position * e.vertexes->at(it->a[0]);
						triangle.a = e.position * e.vertexes->at(it->a[1]) - triangle.base;
						triangle.b = e.position * e.vertexes->at(it->a[2]) - triangle.base;
						if ((triangle.a.crossProduct(triangle.b)).dotProduct(lineSegment.a)<0)
						{
							continue;
						}
	
						float k;
						if (isCrossTrgLine(triangle, lineSegment, k))
						{
							if (k > 0.0 && k < 1.0)
							{
								result++;
							}
						}
					}
				}
			}
		}
	}
	return result;
}

bool OpenGL_Engine::findCollision(const Line&                        line_segment,
                                  const std::set<const GameObject*>& exclude_objects,
                                  OUT GameObject*&                   crossing_object,
                                  OUT v3&                            crossing_position,
                                  OUT Plane&                         crossing_triangle) const
{
	float min_k = INF;

	for EACH_CONST(ShadowPVector, shadows, it)
	{
		GraphShadow& e = *GS(*it);

        if (e.faceMaterials != NULL &&  exclude_objects.find(e.object) == exclude_objects.end())
		{
			for EACH_CONST(FaceMaterialVector, *e.faceMaterials, faces)
			{
				for EACH(TriangleVector, faces->faces->triangles, it)
				{
					Plane triangle;
					triangle.base = e.realPosition * e.vertexes->at(it->a[0]);
					triangle.a = e.realPosition * e.vertexes->at(it->a[1]) - triangle.base;
					triangle.b = e.realPosition * e.vertexes->at(it->a[2]) - triangle.base;
					if ((triangle.a.crossProduct(triangle.b)).dotProduct(line_segment.a)>0)
					{
						continue;
					}

					float k;
					if (isCrossTrgLine(triangle, line_segment, k))
					{
						if (k > 0.0 && min_k > k && k <= 1.0f)
						{
							min_k = k;
							crossing_object = e.object;
							crossing_position = line_segment.point(k);
							crossing_triangle = triangle;
						}
					}
				}
			}
		}
	}
	return min_k < INF;
}


} // namespace opengl
