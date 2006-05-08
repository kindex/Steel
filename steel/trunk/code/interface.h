#ifndef INTERFACE_H
#define INTERFACE_H

#include "math/vector3d.h"
#include "math/matrix4x4.h"

#include <vector>

// Graph+Physic interface
// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];
};

//typedef std::vector<v3>			Vertexes;

// ���������� ������������� ������
typedef int	uid;


class BufferedElement
{
public:
	//protected:
	// true, ���� ���� ������ ��� ������ � �������� ����
	bool	changed;
	// ���������� ������������� ��� ������� ������
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
	Vertexes(): data(0) {}
};

//typedef std::vector<Triangle>	Triangles;
struct Triangles: public BufferedElement
{
	std::vector<Triangle> data;
};


/*
protottype: Interface
�� ����� ������ ����������� ��� ������� � ���������� � ������.

������ ����� ������:
* ����� ��������� (� ����������� ��� ��������� �����������). 
��������� - ������� 4�4, � ������� ���������� �����, ������� � ��������������� ����

*/


class Interface
{
public:
/*������� ������������� ������� ������������ ��� ��������. 
�������� � ���� �������, ����� � �������.
getPVertexes ���������� ���������� ����� � ������� ��������� getPMatrix*/
	virtual	matrix44	getMatrix() = 0;
	virtual	void		setMatrix(matrix44 const &m) = 0; 

	typedef enum
	{
		none,
		local,
		global
	} PositionKind;

    virtual PositionKind	getPositionKind() = 0;
	virtual void changePositionKind(const PositionKind newKind) = 0;
};



#endif

