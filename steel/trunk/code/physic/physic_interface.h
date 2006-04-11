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

#ifndef PHYSIC_INTERFACE_H
#define PHYSIC_INTERFACE_H

#include "../engine.h"
#include "../math/geometry.h"
#include "../math/matrix4x4.h"
#include "../math/maths.h"
#include "../math/vector3d.h"

#include <string>

class PhysicInterface;
typedef std::vector<PhysicInterface*> PhysicInterfaceList;

class PhysicInterface: public Interface
{
public:
	PhysicInterface() {}
/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual PhysicInterfaceList getPChildrens() = 0;

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual bool cleanupP() = 0; // при необходимости отчищает
	virtual Vertexes*	getPVertexes() = 0; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	// массив индексов вершин, которые образуют треугольники (грани)
	virtual Triangles*	getPTriangles() = 0; 

/*матрица трансформации объекта относительно его родителя. 
Включает в себя повотор, сдвиг и масштаб.
getPVertexes возвращяет координаты точек в системе координат getPMatrix*/

	virtual matrix4		getPMatrix() = 0;
	virtual void		setPMatrix(matrix4 const &m) = 0;

	// положение (лучше заменить матрицей, которая включаеи в себя положение, масштаб и поворот тела)
//	virtual v3		getPosition() = 0;
	// возвращает true, если можно изменить положение
//	virtual bool	setPosition(v3 const &v) = 0;

// Может ли объект двигаться при коллизии и действуют ли на него внешние силы, вроде гравитации
	virtual bool	isMovable() = 0;
	virtual bool	isRotatable() = 0;

	// скорость в глобальных коодринатах
	virtual v3		getVelocity() = 0;
	virtual void	setVelocity(v3 const &v) = 0;
	// масса
	virtual	coord	getMass() = 0;

// если объект хочет двигаться к другому объекту, то он возвращяет true 
// и локальные координаты цели движения
	virtual bool	getTarget(v3 &targetPoint, coord &speed) = 0;
// если цель достигнута, то вызывается эта функция
	virtual void	setTargetReached() = 0;
// уникальный идентификатор объекта или пустая строка
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time speed) = 0;
//	virtual std::string getMaterial() = 0;
};

#endif

