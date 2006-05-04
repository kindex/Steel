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
#include "../math/geometry.h"
#include "../math/maths.h"
#include "../math/vector3d.h"

#include "../res/material/material.h"

struct Light
{
	v3 pos;
	float intensivity, range;
};

/*
Sprite, Billboard. Полигон, который всегда параллелен плоскости экрана.
*/

typedef std::vector<Light>		Lights;

class MapCoord: public BufferedElement
{
public:
	std::vector<v2>	data;

//	kind : 1d, 2d, 3d
};

typedef Vertexes	Normals;


typedef MapCoord MapCoords;
//typedef std::vector<v2> MapCoords;


// материал + треугольники, к которым он относится
struct FaceMaterial
{
	Material		*material;
	Triangles		triangles;
	std::string		name;
};

typedef std::vector<FaceMaterial>	FaceMaterials;


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
typedef std::vector<GraphInterface*> GraphInterfaceList;

class GraphInterface: public Interface
{
public:
/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual GraphInterfaceList getChildrens() = 0;

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getFrame() = 0; // AABB of object
/*Форма объектя для рисования (без формы потомков)
 full shape for physics, graph (polygon, 3D mesh)
 quality = 0 - aabb
 quality = 1 - со всеми деталями
 В зависимости от расстояния и производительности машины.


NURBS ?
Decals ?

Прогаммно сгенерированные текстуры? no
Video textures (avi, camera)

*/
	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual bool cleanup() = 0; // при необходимости отчищает 
	virtual Vertexes*	getVertexes() = 0; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	virtual Normals*	getNormals() = 0; // список нормалей в вершинам

	// массив индексов вершин, которые образуют треугольники (грани)
//	virtual Triangles*	getTriangles() = 0; 

	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual MapCoords*	getMapCoords() = 0;

	virtual Lights*		getLights() = 0;

	virtual	uid			getId() = 0;
	virtual	void		processGraph(v3	cameraEye, v3 cameraDirection) = 0;
};

#endif

