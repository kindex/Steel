#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include "../engine.h"
#include "../math/maths.h"
#include "../math/vector3d.h"
#include "../math/geometry.h"

// RES
#include "../res/model/model.h"

/********************************* GRAPH **********************************************/




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
typedef vector<GraphInterface*> GraphInterfaceList;

class GraphInterface: public Interface
{
public:
/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual GraphInterfaceList getChildrens() = 0;

/*Любая точка внутри объекта*/
	virtual v3 getPos() = 0; // return center of object, origin
/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getFrame() = 0; // AABB of object
/*Форма объектя для рисования (без формы потомков)
 full shape for physics, graph (polygon, 3D mesh)
 quality = 0 - aabb
 quality = 1 - со всеми деталями
 В зависимости от расстояния и производительности машины.
 */
	//virtual GShape getGShape(float quality=1) = 0; 
	/*
Vertex
Face
Material
Sprites ?
NURBS ?
Lights
Decals ?

Прогаммно сгенерированные текстуры? no
Video textures (avi, camera)

*/
	// Интерфейс заточен под 3DS и OpenGL
	// Следующие функции возврящяют ссылки на массивы данных (NULL if none), и должны 
	// отвечать за хранние этих данных до следующего вызова этой функции
	// или вызова cleanup
	virtual bool cleanup() = 0; // при необходимости отчищает 
	virtual vertexes*	getVertexes() = 0; // список вершин. 
	// массив индексов вершин, которые образуют треугольники (грани)
	virtual triangles*	getTriangles() = 0; 
	// материал каждой грани
//	virtual faces*	getFaces() = 0; // NULL if none

};


class GraphEngine: public Engine
{
protected:
	// data, to store collected information
	vertexes	vertex;
	triangles	triangle;
public:
	// Collect information about object: how to render it
	virtual bool inject(GraphInterface *object);
	// Draw colelcted information. May be called few times without recollection information
	bool process() = 0; // Override OpenGL, D3D, ...\
	// Clear collected information
	bool clear()
	{
		vertex.clear();
		triangle.clear();
		return true;
	}
};




#endif

