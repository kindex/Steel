/*id*********************************************************
	File: graph/graph_interface.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Bond [Andrey Bondarenko]
	License:
		Steel Engine License
	Description:
		Графический джижок - интерфейс рисуемого объекта
 ************************************************************/

#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include "../steel.h"
#include "../engine/engine.h"
#include "../engine/interface.h"
#include "graph_types.h"

// материал + треугольники, к которым он относится

/*
Интерфейс для всех объектов в проекте, которые должны рисоваться через 
страндартный рендерер. Объекты составные. Родитель возвращает список
прикреплённых к нему объектов. В каркас (aabb) входит также и каркасы потомков.
Это нужно для быстрого отсечения невидимых объектов.
Графический интерфейс, в отличае от физического, только считывает информацию.
Графика рисует, физика меняет объекты.
По идее, объект может и не брать координаты и форму из памяти, а генерировать их на ходу,
например, как мета-шарики, система частиц
*/

class GraphLightInterface
{
public:
	virtual void addLight(Light*) abstract;
	virtual void removeLight(uid) abstract;
	virtual void updateLight(uid, Light*) abstract;
};

class GraphInterface : 
    public ChildrenInterface, 
    public Interface3D, 
    public GraphLightInterface,
    public Engine
{
public:

// *** Configuration ***

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
//	virtual aabb getFrame() { return = 0; // AABB of object

	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual void	setVertexes(const VertexVector*) abstract; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	virtual void	setNormals(const Normals*) abstract; // список нормалей в вершинам

	virtual void	setLines(const GLines*) abstract; // индексы вершин для линий и цвета линий (for debug)

	// массив индексов вершин, которые образуют треугольники (грани) + материалы
	virtual void	setFaceMaterials(const FaceMaterialVector*) abstract;
	virtual void	setTexCoordsCount(unsigned int) abstract;
	virtual void	setTexCoords(unsigned int texNumber, const TexCoords*) abstract;
	virtual void	setAABB(const AABB&) abstract;
	virtual const ProcessInfo& getProcessInfo() abstract;
};

#endif

