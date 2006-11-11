#ifndef ENGINE__ENGINE_TYPES_H
#define ENGINE__ENGINE_TYPES_H

#include "../steel.h"

#include "../math/vector3d.h"
#include "../math/matrix34.h"
#include "../common/steel_vector.h"
#include "id_generator.h"


// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];

	void set(unsigned int _0, unsigned int _1, unsigned int _2) 
	{ 
		a[0] = _0;
		a[1] = _1;
		a[2] = _2;
	}
};

// ћассив значений, который может быть кеширован в графическом ускорителе
class BufferedElement
{
public:
	// true, если этот массив был изменЄн с прошлого раза
	bool	changed;
	// уникальный идентификатор дл€ массива данных
	uid		id;
public:
	uid		getId(void) const { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged(void) const { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }
};

// массив вершин многогранника
struct Vertexes: public BufferedElement
{
	steel::vector<v3> data;

	Vertexes(): data(0) {}
};

// массив треугольников многогранника
struct Triangles: public BufferedElement
{
	steel::vector<Triangle> data;
};

/*
местоположение + поворот объекта
*/
typedef matrix34 ObjectPosition;

// “ип положени€ объекта: local Ц относительно родитил€, global Ц в глобальной системе координат.
typedef enum
{
	POSITION_NONE,
	POSITION_LOCAL,
	POSITION_GLOBAL,
	POSITION_SCREEN
} PositionKind;


#endif
