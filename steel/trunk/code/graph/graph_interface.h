/*id*********************************************************
	Unit: GraphEngine
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Bond [Andrey Bondarenko]
	License:
		Steel Engine License
	Description:
		Графический джижок - интерфейс рисуемого объекта
 ************************************************************/

#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include "../engine.h"

#include "types.h"

#include "../res/material/material.h"

// материал + треугольники, к которым он относится
struct FaceMaterial
{
	Material		*material;
	Triangles		*triangles;
	std::string		name;
};

typedef steel::vector<FaceMaterial>	FaceMaterials;


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

class GraphObject;
typedef steel::svector<GraphObject*> GraphObjectList;

class GraphObject: public Interface
{
protected:
	int gInjectedCount;
public:
// *** Common ***
	GraphObject(): gInjectedCount(0) {}
	// список детей
	/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	// возвращает количество детей
	virtual int getGraphChildrenCount(void) { return 0; } 
	// ребёнок с указанным номером
	virtual GraphObject* getGraphChildren(int number) { return NULL; }

	// Непосредственно перед добавлением в движок вызывается 
	virtual bool GraphBeforeInject() { gInjectedCount++; return true;}
	// После удаления из движка вызывается процедура afterRemove
	virtual void GraphAfterRemove() {gInjectedCount--;}

	// вызывается перед каждой итерацией обработки. Внутри этой процедуры объект может менять некоторые свои параметры
	virtual	void ProcessGraph(const GraphEngineInfo &info) {}

// *** Configuration ***

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
//	virtual aabb getFrame() { return = 0; // AABB of object

	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual Vertexes*	getVertexes() { return NULL;} // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	virtual Normals*	getNormals() { return NULL;} // список нормалей в вершинам

	virtual GLines*		getLines() { return NULL;} // индексы вершин для линий и цвета линий (for debug)

	// массив индексов вершин, которые образуют треугольники (грани) + материалы
	virtual FaceMaterials* getFaceMaterials() { return NULL;}
	virtual TexCoords*	getTexCoords(int texNumber) { return NULL;}

	virtual Lights*		getLights() { return NULL;}
};

#endif

