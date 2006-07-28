/*id*********************************************************
    Unit: GraphEngine - OpenGL Engine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Функции для рендерига объектов на OpenGL 1.1.
		Используется glVertexPointer и BindTexture
 ************************************************************/

#ifndef __GRAPH_OPENGL_11_H
#define __GRAPH_OPENGL_11_H

#include "opengl_engine.h"

void DrawFill_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, Material *material, GraphEngine::GraphTotalInfo &total);
void DrawWire_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, Triangles *triangles, GraphEngine::GraphTotalInfo &total);
void DrawLines_OpenGL11(OpenGL_Engine::GraphObjectStorage &e, GraphEngine::GraphTotalInfo &total);

#endif // #ifndef __GRAPH_OPENGL_11_H
