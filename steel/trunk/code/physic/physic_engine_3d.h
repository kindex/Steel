/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
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
		v3	normal; // плоскость, в которой произошла коллизия
		v3	point; // точка коллизии
		float	time; // время (от начала карда) в процентах [0..1]
		steel::time globalTime;
		PhysicInterface *a, *b; // учатники коллизии

		bool operator<(const Collision second) const
		{
			return a<second.a || a == second.a && b < second.b;
		}
	};

	std::map<Collision, int> allCollisions;

	v3 g;
//	std::map<std::string, int> tag;
public:
	PhysicEngine3D() { helper = NULL; conf = NULL; }
	bool process(steel::time globalTime, steel::time time);

	bool process(PhysicInterface &o, steel::time globalTime, steel::time time);
	bool moveObject(PhysicInterface &o, v3 oldPos, v3 direction /*global*/, float &processedTime);
	bool rotateObject(PhysicInterface &o, matrix44 oldMatrix, v3 rotAxis /*local*/, float &processedTime);

	bool init(std::string _conf);
	bool prepare(PhysicInterface *object, matrix44 matrix = matrix44::getIdentity(), PhysicInterface *parent = NULL);
	bool update(Element &element);
	bool clear();
// CD
	void collisionDetection(PhysicInterface &a, PhysicInterface &b, v3 distance, Collision &collision, PhysicInterface *clip);
	void collisionDetectionRotation(PhysicInterface &a, PhysicInterface &b, const matrix44 rotation, Collision &collision, PhysicInterface *clip);
	void collisionDetection(PhysicInterface &o, v3 distance, Collision &collision, PhysicInterface *clip);
	void collisionDetectionRotation(PhysicInterface &o, const matrix44 rotation, Collision &collision, PhysicInterface *clip);

	bool checkCollision(PhysicInterface &a, v3 distance, PhysicInterface &b, Collision &collision);
	bool checkCollisionRotation(PhysicInterface &a, const matrix44 rot, PhysicInterface &b, Collision &collision);

	bool checkCollisionMTrgTrg(Plane a, v3 direction, Plane b, Collision &collision);
	bool checkCollisionRTrgTrg(Plane a, v3 d1, v3 d2, v3 d3, Plane b, Collision &collision);

	void checkCollisionMVertexTrg(const v3 point,	const v3 direction, const Plane b,	Collision &collision);

	void checkCollisionMTrgVertex(const Plane a,	const v3 direction, const v3 point, Collision &collision);
	void checkCollisionRTrgVertex1up(const Plane a, v3 d1, v3 d2, v3 d3, const v3 point, Collision &collision);
	void checkCollisionRTrgVertex2up(const Plane a, v3 d1, v3 d2, v3 d3, const v3 point, Collision &collision);
	void checkCollisionRTrgVertex3up(const Plane a, v3 d1, v3 d2, v3 d3, const v3 point, Collision &collision);

	void checkCollisionMLineTrg  (const Line a,		const v3 direction, const Plane b,	Collision &collision);
	void checkCollisionRLineTrg1up(const Line a,		v3 d1, v3 d2, const Plane b,	Collision &collision);
	void checkCollisionRLineTrg2up(const Line a,		v3 d1, v3 d2, const Plane b,	Collision &collision);

	void checkCollisionMLineLine (const Line a,		const v3 direction, const Line b,	Collision &collision);

	void checkCollisionRLineLine1up(const Line a,	v3 d1, v3 d2, const Line b,	Collision &collision);
	void checkCollisionRLineLine2up(const Line a,	v3 d1, v3 d2, const Line b,	Collision &collision);
// CR
	bool collisionReaction(Collision collision);
	bool collisionReactionUniUni(const Collision collision);
	bool collisionReactionUniNone(const Collision collision);

	void incCollision(const Collision collision);
	int findCollision(const Collision collision);
};


#endif
