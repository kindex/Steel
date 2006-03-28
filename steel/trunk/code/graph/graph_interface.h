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
#include "../math/matrix4x4.h"
#include "../math/maths.h"
#include "../math/vector3d.h"


struct Light
{
	v3 pos;
	float intensivity, range;
};


typedef std::vector<v3>			Normals;
typedef std::vector<v2>			MapCoords;
typedef std::vector<Light>		Lights;
typedef std::map<std::string, std::vector<int> >  FaceMaterials;

struct color_24
{
    unsigned char r,g,b;
};

struct color_f
{
    float r,g,b;
    color_f() {}
    color_f(float R, float G, float B): r(R), g(G), b(B) {}
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


Sprites ?
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
	virtual Triangles*	getTriangles() = 0; 

	virtual MapCoords*	getMapCoords() = 0; 
	virtual FaceMaterials* getFaceMaterials() = 0;
	virtual Lights* getLights() = 0;

/*?? Любая точка внутри объекта. Относительно этой точки рисуется оьъект (координатывершин)*/
	virtual MATRIX4X4 getMatrix() = 0;
};

#endif

