/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Функции для рендерига объектов на OpenGL 1.0
		Используется минимальный набор функциональности
		glBegin(GL_TRIANGLES) и не испоьзуется BindTexture.
		Этот рендерер должен работать на любой машине.
 ************************************************************/

#ifndef __GRAPH_OPENGL_10_H
#define __GRAPH_OPENGL_10_H

#include "opengl_engine.h"

void DrawFill_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
void DrawWire_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total);
void DrawLines_OpenGL10(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);

#endif // #ifndef __GRAPH_OPENGL_10_H
