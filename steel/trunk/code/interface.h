#ifndef INTERFACE_H
#define INTERFACE_H

#include "math/vector3d.h"

#include <vector>

// Graph+Physic interface
// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];
};

//typedef std::vector<v3>			Vertexes;

// уникальный идентификатор модели
typedef int	uid;


struct BufferedElement
{
//protected:
	// true, если этот массив был изменён с прошлого раза
	bool	changed;
	// уникальный идентификатор для массива данных
	uid		id;
public:
	uid		getId() { return id; }
	void	setId(uid _id) { id = _id; changed = false; }
	bool	wasChanged() { return changed; }
	void	setChanged(bool _changed) {changed = _changed; }
};


struct Vertexes: public BufferedElement
{
	std::vector<v3> data;
};

//typedef std::vector<Triangle>	Triangles;
struct Triangles: public BufferedElement
{
	std::vector<Triangle> data;
};


/*
protottype:
*/
class Interface{};



#endif

