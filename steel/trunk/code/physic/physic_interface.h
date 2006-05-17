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
#include "../math/aabb.h"
#include "../math/matrix4x4.h"
#include "../math/vector3d.h"

#include <string>

class PhysicInterface;
typedef std::vector<PhysicInterface*> PhysicInterfaceList;

class PhysicEngine;

namespace ProcessKind
{
	typedef enum
	{
		none,
		custom, // сам объект определяет своё движение
		uni // движется по универсальным законам
	} ProcessKind;
};

namespace CollisionType
{
	typedef enum
	{
		none,
		uni,
		trigger
	} CollisionType;
};

namespace ShapeType
{
	typedef enum
	{
		none,
		poly,
		sphere
	} ShapeType;
}

struct velocity
{
	v3 translation; // скорость поступательного движения
	v3 rotationAxis;  // ось вращения
	float rotationSpeed; // скорость вращения (радиан/сек)

	inline velocity() {}
	inline velocity(const v3 _translation, const v3 _axis, const float _rotSpeed) 
	{
		translation = _translation;
		rotationAxis = _axis;
		rotationSpeed = _rotSpeed;
	}
};

class PhysicInterface: public Interface
{
	steel::time	currentTime; // время, в котором находиться объект
	friend class PhysicEngine;
	friend class PhysicEngine3D;

public:
	virtual ProcessKind::ProcessKind getProcessKind() = 0;

	PhysicInterface() {}
/*	список составных частей объекта (потомков). Например, для мира - это стены и монстры, а для монстра это может быть частами тела.*/
	virtual PhysicInterfaceList getPChildrens() = 0;

/*Каркас - прямоугольник, в котором содержится объект. Может быть больше, но не меньше пространства, занимаемым обхектом. Должен вычисляться быстро*/
	virtual aabb getPFrame() = 0; // AABB of object
	virtual bool cleanupP() = 0; // при необходимости отчищает
	virtual Vertexes*	getPVertexes() = 0; // список вершин (координаты отночительно матрицы getMatrix() и всех матриц предков)
	// массив индексов вершин, которые образуют треугольники (грани)
	virtual Triangles*	getTriangles() = 0; 

/*матрица трансформации объекта относительно его родителя. 
Включает в себя повотор, сдвиг и масштаб.
getPVertexes возвращяет координаты точек в системе координат getPMatrix*/

	// положение (лучше заменить матрицей, которая включаеи в себя положение, масштаб и поворот тела)
//	virtual v3		getPosition() = 0;
	// возвращает true, если можно изменить положение
//	virtual bool	setPosition(v3 const &v) = 0;

// Может ли объект двигаться при коллизии и действуют ли на него внешние силы, вроде гравитации
	virtual bool	isMovable() = 0;
	virtual bool	isRotatable() = 0;

	// скорость в глобальных коодринатах
	virtual velocity	getVelocity() = 0;
	virtual void	setVelocity(const velocity &v) = 0;
	virtual matrix44	getGlobalMatrix() = 0;
	virtual velocity	getGlobalVelocity() = 0;
	virtual	float		getGlobalScale() = 0;


	// масса
	virtual	coord	getMass() = 0;

// уникальный идентификатор объекта или пустая строка
	virtual std::string getName() = 0;
	
	virtual	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine) = 0;
	virtual	v3		getPosition() = 0;
	virtual	bool	setPosition(v3 const &v) = 0;
//	virtual std::string getMaterial() = 0;

	// эта функция вызывается, еслу другой объект трогает этот
	virtual void	trigger(PhysicInterface *object) = 0;
};

#endif

