/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Kinematics, Collision Detection and Collision Reaction
	TODO
		delete file
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
		bool fake; // false collision
		Collision() { fake = true; }

		bool operator<(const Collision second) const
		{
			return a<second.a || a == second.a && b < second.b;
		}
	};

	typedef std::map<Collision, int> collisionSet;
	
	collisionSet allCollisions, lastCollisions;

//	std::map<std::string, int> tag;
public:
	PhysicEngine3D() { helper = NULL; conf = NULL; }

	bool process(steel::time globalTime, steel::time time);

	bool process(PhysicInterface &o, steel::time globalTime, steel::time time);
	bool moveObject(PhysicInterface &o, v3 oldPos, v3 direction /*global*/, float &processedTime);
	bool rotateObject(PhysicInterface &o, matrix34 oldMatrix, v3 rotAxis /*local*/, float &processedTime);

	bool init(std::string _conf);
	bool prepare(PhysicInterface *object, matrix34 matrix = matrix34::getIdentity(), PhysicInterface *parent = NULL);
	bool clear();
// CD
	bool checkInvariant(PhysicInterface &o, PhysicInterface &clip);
	bool checkInvariant(PhysicInterface &a, PhysicInterface &b, PhysicInterface &clip);
	bool crossModelModel(PhysicInterface &a, PhysicInterface &b);
	bool intersectModelModel(PhysicInterface &a, PhysicInterface &b);
	bool crossPointModel(v3 point, PhysicInterface &b);


	void collisionDetection(PhysicInterface &a, PhysicInterface &b, v3 distance, Collision &collision, PhysicInterface *clip);
	void collisionDetectionRotation(PhysicInterface &a, PhysicInterface &b, const matrix34 rotation, Collision &collision, PhysicInterface *clip);
	void collisionDetection(PhysicInterface &o, v3 distance, Collision &collision, PhysicInterface *clip);
	void collisionDetectionRotation(PhysicInterface &o, const matrix34 rotation, Collision &collision, PhysicInterface *clip);

	bool checkCollision(PhysicInterface &a, v3 distance, PhysicInterface &b, Collision &collision);
	bool checkCollisionRotation(PhysicInterface &a, const matrix34 rot, PhysicInterface &b, Collision &collision);

	bool checkCollisionMTrgTrg(Plane a, v3 direction, Plane b, Collision &collision, PhysicInterface &aobj, PhysicInterface &bobj);
	bool intersectTrgTrg(Plane a, Plane b);
	bool intersectLineTrg(Line a, Plane b);
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

	void applyImpule(PhysicInterface &a, v3 point, v3 impulse);

	void incCollision(const Collision collision);
	int findCollision(const collisionSet col, const Collision collision);
};

#endif
