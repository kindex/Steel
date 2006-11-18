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

typedef steel::vector<FaceMaterial>	FaceMaterials;

class ChildrenInterface
{
public:
	virtual void addChild(GameObject* child) = 0;
	virtual void deleteChild(GameObject* child) = 0;
	virtual void clearChildren(void) = 0;
};

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

class GraphInterface: public BaseInterface, public ChildrenInterface
{
public:
	static const InterfaceId interfaceId = 0x100;

	virtual void setPosition(ObjectPosition) = 0;
	virtual void setPositionKind(PositionKind) = 0;

// *** Configuration ***

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
//	virtual aabb getFrame() { return = 0; // AABB of object

	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual void	setVertexes(Vertexes*) = 0; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	virtual void	setNormals(Normals*) = 0; // список нормалей в вершинам

	virtual void	setLines(GLines*) = 0; // индексы вершин для линий и цвета линий (for debug)

	// массив индексов вершин, которые образуют треугольники (грани) + материалы
	virtual void	setFaceMaterials(FaceMaterials*) = 0;
	virtual void	setTexCoordsCount(int) = 0;
	virtual void	setTexCoords(int texNumber, TexCoords*) = 0;

	virtual void 	setLights(Lights*) = 0;
};

#endif

