/*id*********************************************************
    Unit: OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Функции для рендерига объектов на OpenGL 2.0

		Shader Objects
 ************************************************************/

#include "opengl_glsl.h"
#include "opengl_engine.h"
#include "gl/libext.h"

// нарисовать множество полигонов с указанным материалом / Multitexture
void OpenGL_Engine::DrawFill_OpenGL20(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total)
{
	if(material && GL_EXTENSION_GLSL)
	{
		if(!material->isShader())
		{
			OpenGL_Engine::DrawFill_OpenGL13(e, triangles, material, total);
			return;
		}

		total.object++;

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		BindShader(material);

		if(DrawTriangles) (this->*DrawTriangles)(e, triangles, NULL, total);

	   	glPopAttrib();
	}
}


bool OpenGL_Engine::BindShader(Material *material)
{
	if(!material->isShader()) return false;

	uid id = material->getId();

	OpenGL_Buffer &buf = buffer[id];

	if(!buf.loaded)
	{
		GLSL *shader = new GLSL;
		if(shader->init(material))
		{
			buf.loaded = true;
			buf.glid = shader->getGL_Id();
			buf.object = (char*)shader;
		}
	}

	if(buf.loaded)
	{
		// setup variables
		glUseProgramObjectARB(buf.glid);
		return true;
	}
	return false;
	
}
