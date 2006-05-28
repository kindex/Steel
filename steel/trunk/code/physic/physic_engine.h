/*id*********************************************************
    Unit: PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		TODO
 ************************************************************/

#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H

#include "../res/conf/conf.h"
#include "../math/matrix4x4.h"

#include "physic_interface.h"

class PhysicEngine: public Engine
{
protected:
	struct Element
	{
		PhysicInterface	*object, *parent;
		matrix44		matrix, parentMatrix; // abloslute
		Vertexes		*vertex;
		Triangles		*triangle;
		aabb			frame;
		int				collisionCount;
	};

	std::vector<Element> element;
	std::vector<PhysicInterface*> object;

public:
	struct TotalInfo
	{
		int collisionCount;
	} total;

public:
	// Inject добавляет объект для обработки
	// Типы движений объектов: uni, custom, none
	// У uni не может быть детей
	// У custom и none не может быть детей с типом uni
	virtual bool inject(PhysicInterface *object);

	virtual bool prepare(PhysicInterface *object, matrix44 matrix = matrix44::getIdentity(), PhysicInterface *parent = NULL) = 0;
	virtual bool update(Element &element) = 0;

	// Move objects
	virtual bool process(steel::time globalTime, steel::time time) = 0; 
	// Clear collected information
	virtual bool clear();

	virtual bool init(std::string _conf);
	virtual bool deinit() { return clear();}
};

#endif

