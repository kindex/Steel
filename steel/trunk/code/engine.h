/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
		* Bond [Andrey Bondarenko]
    Licence:
        ������ ��� Division
    Description:
		����� Interface - �������� ������, �� �������� ������ ������������� 
		������, ������� ������ �������������� ������� Engine.
		����� Engine (������) ����� ������� inject() �������� �������, �������
		������ ���� ����������, �� ������������ �� ������ ������ process().
		��� ����� ������� ������ ������������ ��������� (interface) ������ 
		����������� � �������.
 ************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include "steel.h"
#include "res/res.h"
#include "res/conf/conf.h"
#include <string>
#include "math/vector3d.h"
#include <vector>

// Graph+Physic interface
// One triangle, indexes of vertexes
struct Triangle // anti-clockwise direction (one side Triangle)
{
	unsigned int a[3];
};

//typedef std::vector<v3>			Vertexes;
struct Vertexes
{
	std::vector<v3> vertex;
	uid		id;
	uid getId() { return id; }
};

typedef std::vector<Triangle>	Triangles;


/*
protottype:
*/
class Interface{};


class Engine: public steelAbstract
{
protected:
	ResCollection	*res;
public:
	Config			*conf;

	virtual ~Engine() {}
	virtual bool init(std::string _conf) = 0; // initialization

	virtual bool clear() = 0; 
//	protottype: virtual bool inject(Interface *object) { return false; }; // add object to process by engine
//	virtual	bool remove(Interface *object);
//	virtual bool process() = 0; // add object to process by engine
	virtual bool deinit() = 0;

	// TODO: ������ ���. �� ��������� � ���� ������
	void bindResColelntion(ResCollection *_res) { res = _res; }
};

#endif
