/*id*********************************************************
	File: graph/graph_interface.h
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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
#include "material.h"

#include "material.h"

// материал + треугольники, к которым он относится
struct FaceMaterial
{
	Material		*material;
	Triangles		*triangles;
	std::string		name;
};

typedef svector<FaceMaterial>	FaceMaterials;

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
	virtual void addLight(Light*) = 0;
	virtual void removeLight(uid) = 0;
	virtual void updateLight(uid, Light*) = 0;
};

class GraphInterface: public BaseInterface, public ChildrenInterface, public Interface3D, public GraphLightInterface
{
public:
	static const InterfaceId interfaceId = 0x100;

// *** Configuration ***

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
//	virtual aabb getFrame() { return = 0; // AABB of object

	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual void	setVertexes(const Vertexes*) = 0; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	virtual void	setNormals(const Normals*) = 0; // список нормалей в вершинам

	virtual void	setLines(const GLines*) = 0; // индексы вершин для линий и цвета линий (for debug)

	// массив индексов вершин, которые образуют треугольники (грани) + материалы
	virtual void	setFaceMaterials(const FaceMaterials*) = 0;
	virtual void	setTexCoordsCount(unsigned int) = 0;
	virtual void	setTexCoords(unsigned int texNumber, const TexCoords*) = 0;
	virtual void	setAABB(const AABB&) = 0;
};

#endif
