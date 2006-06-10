/*id*********************************************************
    Unit: GraphEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
		* Bond [Andrey Bondarenko]
    Licence:
        Только для Division
    Description:
		Графический джижок - интерфейс рисуемого объекта
 ************************************************************/

#ifndef __GRAPH_INTERFACE_H
#define __GRAPH_INTERFACE_H

#include "../engine.h"
#include "../math/aabb.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/vector2d.h"

#include "../res/material/material.h"

struct Light
{
	v3 pos;
	float intensivity, range;
};

typedef steel::vector<Light>		Lights;

//	kind : 2d only
class TexCoord: public BufferedElement
{
public:
	steel::vector<v2>	data;
};

class GLine // al-a triangle
{
public:
	int a[2]; // vertex indexes
	v4 color; // line color
	void set(const int A, const int B) { a[0] = A; a[1] = B; }
};

typedef steel::vector<GLine> GLines;

typedef Vertexes	Normals;

typedef TexCoord TexCoords;

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

class GraphInterface;
typedef steel::vector<GraphInterface*> GraphInterfaceList;

class GraphInterface: public Interface
{
public:
/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual GraphInterfaceList getChildrens() = 0;

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getFrame() = 0; // AABB of object

	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual bool cleanup() = 0; // при необходимости отчищает 
	virtual Vertexes*	getVertexes() = 0; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	virtual Normals*	getNormals() = 0; // список нормалей в вершинам

	virtual GLines*	getLines() = 0; // индексы вершин для линий и цвета линий (for debug)

	// массив индексов вершин, которые образуют треугольники (грани) + материалы
	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual TexCoords*	getTexCoords(int texNumber) = 0;

	virtual Lights*		getLights() = 0;

	virtual	void		processGraph(v3	cameraEye, v3 cameraDirection) = 0;
};

#endif

