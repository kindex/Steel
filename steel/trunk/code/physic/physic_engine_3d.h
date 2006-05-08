/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        ������ ��� Division
    Description:
		TODO
 ************************************************************/

#ifndef PHYSIC_ENGINE_3D_H
#define PHYSIC_ENGINE_3D_H

#include "physic_engine.h"
#include "../math/plane.h"

#include <map>
#include <string>


class PhysicEngine3D: public PhysicEngine
{
protected:
	struct Collision
	{
		v3	normal; // ���������, � ������� ��������� ��������
		v3	point; // ����� ��������
		float	time; // ����� (�� ������ �����) � ��������� [0..1]
		Element *a, *b; // �������� ��������
	};


	v3 g;
//	std::map<std::string, int> tag;
public:
	bool process(steel::time globalTime, steel::time time);
	bool init(std::string _conf);
	bool prepare(PhysicInterface *object, matrix44 matrix = matrix44::getIdentity(), PhysicInterface *parent = NULL);
	bool clear();

	bool collisionDetection(Element &el, v3 distance, Collision &collision);
	bool checkCollision(Element &a, v3 distance, Element &b, Collision &collision);
	bool checkCollisionMTrgTrg(Plane a, v3 direction, Plane b, Collision &collision);

	void checkCollisionMVertexTrg(const v3 point,	const v3 direction, const Plane b,	Collision &collision);
	void checkCollisionMTrgVertex(const Plane a,	const v3 direction, const v3 point, Collision &collision);
	void checkCollisionMLineTrg  (const Line a,		const v3 direction, const Plane b,	Collision &collision);
	void checkCollisionMLineLine (const Line a,		const v3 direction, const Line b,	Collision &collision);
};


#endif
