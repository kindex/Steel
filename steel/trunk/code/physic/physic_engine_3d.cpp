/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Kinematics, Collision Detection and Collision Reaction
 ************************************************************/

#include "physic_engine_3d.h"

#include "../math/plane.h"
#include "../common/utils.h"

using namespace std;

#define CONTACT_EPSILON (0.01f)

// Сердце физического движка
// обрабатывает движение одного объекта в указанный промежуток времени

bool PhysicEngine3D::moveObject(PhysicInterface &o, v3 oldPos, v3 dir /*global*/, float &processedTime)
{
	Collision collision; collision.time = INF; // no collision

	collisionDetection(o, dir, collision, &o);

	if(collision.time<=1 + EPSILON)
	{
		float len = dir.getLength();
		len *= collision.time;
		len -= CONTACT_EPSILON;
		if(len>EPSILON)
		{
			dir = dir.getNormalized()*len;
			o.setPosition(oldPos + dir);
		}

		collision.a = &o;
		int cnt = findCollision(allCollisions, collision);

		if(cnt == 0 && !collision.fake
//			&& (collision.time>EPSILON || findCollision(lastCollisions, collision) == 0)
			)
		{
			if(helper) // draw collision
				helper->drawVector(Line(collision.point, collision.normal.getNormalized()*0.1f), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));

			collisionReaction(collision);	incCollision(collision); total.collisionCount++;
			processedTime = collision.time;
			
			return false;
		}
		else
		{
			if(cnt ==0 && helper) // draw collision
				helper->drawVector(Line(collision.point, collision.normal.getNormalized()*0.1f), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));
			processedTime = 0;
			return true; // движение закончено - объект столкнулся с дургим объектом повторно
		}
	}
	else
	{
		o.setPosition(oldPos + dir);
	}
	processedTime = 1;
	return true;
}

bool PhysicEngine3D::rotateObject(PhysicInterface &o, matrix44 oldMatrix, v3 rotAxis /*local*/, float &processedTime)
{
	Collision collision; collision.time = INF; // no collision

	matrix44 path;

	path.setRotationAxis(rotAxis.getLength(), rotAxis.getNormalized());

	collisionDetectionRotation(o, path, collision, &o);


	if(collision.time<=1 + EPSILON)
	{
		collision.a = &o;

		if(findCollision(allCollisions, collision) == 0)
		{
			if(helper) // draw collision
				helper->drawVector(Line(collision.point, collision.normal.getNormalized()*0.1f), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));

			collisionReaction(collision);	incCollision(collision); total.collisionCount++;
			processedTime = 0;
			return false;
		}
		else
			return true; // движение закончено - объект столкнулся с дургим обхектом повторно
		return true;
	}
	else
	{
		o.setMatrix(o.getMatrix() * path);
	}
	return true;

}

// обрабатывает движение одного объекта и его потомков
// если была коллизия, то двигает тело еще раз после коллизии (5 раз)
bool PhysicEngine3D::process(PhysicInterface &o, steel::time globalTime, steel::time time)
{
	ProcessKind::ProcessKind kind = o.getProcessKind();
	if(kind != ProcessKind::none) 
	{
		if(kind == ProcessKind::custom) // custom velocity calculations
			o.process(globalTime, time, this);

		float endProcessedTime = 0.0f; // %
		float totalProcessedTime = 0.0f; // %
		float firstProcessedTime = 0.0f; // %
		int collisionCount = 0;
		bool ended;
		do
		{
			const matrix44 global = o.getGlobalMatrix();
			velocity v = o.getVelocity();
			v3 path = v.translation*(float)time;
			v3 oldPos = o.getPosition();
			v3 newPos = oldPos + path;

			Interface::PositionKind pos = o.getPositionKind();
			v3 dir; // global

			if(pos == Interface::local)
				dir = global*newPos-global*oldPos;
			else
				dir = newPos - oldPos;

			ended = moveObject(o, oldPos, dir, endProcessedTime);
			totalProcessedTime += (1-totalProcessedTime)*endProcessedTime;

			if(collisionCount == 0)
				firstProcessedTime = totalProcessedTime;

			collisionCount++;
		}
		while(!ended && collisionCount<4);

		if(totalProcessedTime<0.1)
		{
			velocity v = o.getVelocity();
			v.translation *= 0.75;
			o.setVelocity(v);
		}

//		velocity startv = o.getVelocity();
//		v3 gravitation = ;
		if(kind == ProcessKind::uni) // forces
		{
			velocity v = o.getVelocity();
			v.translation += g*(float)time*clamp(firstProcessedTime	);	// gravitation
			o.setVelocity(v);
		}


/*		velocity endv = o.getVelocity();
		if(startv.translation.z<0 && endv.translation.z>0)
		{ 
			if(endv.translation.z < gravitation.z*3)
			{
				endv.translation.z = 0;
				o.setVelocity(endv);
			}
		}*/

		// Rotation
		if(conf->geti("kiRotation"))
		{
			velocity v = o.getVelocity();
			if(v.rotationAxis.getSquaredLengthd()>EPSILON2) // rotation
			{
				rotateObject(o, o.getMatrix(), v.rotationAxis*(float)time, endProcessedTime);
			}
		}
	}

// move childrens
	PhysicInterfaceList c = o.getPChildrens();
	for(PhysicInterfaceList::const_iterator it = c.begin(); it != c.end(); it++)
		process(**it, globalTime, time);

	if(helper && o.getProcessKind() != ProcessKind::none) // draw velocity
		helper->drawVector(
					Line(o.getGlobalMatrix()*v3(0,0,0),	
					o.getGlobalVelocity().translation*0.1f), 0.0f,0.0f, v4(0.0f,1.0f,0.0f,1.0f));

	if(helper) // draw AABB
	{
		aabb frame = o.getPFrame();
		if(!frame.empty())
		{
			frame.mul(o.getGlobalMatrix());
			helper->drawBox(frame, 0,0, v4(1,1,1, 0.5f));
		}
	}

	return true;
}

// обрабатывает движение всех объектов
bool PhysicEngine3D::process(steel::time globalTime, steel::time time)
{
	if(helper) // draw velocity
		helper->setTime(globalTime);

	lastCollisions = allCollisions;
	allCollisions.clear();

/*	element.clear();*/
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		prepare(*it);

	static int framepass = 0;

	framepass++;

	if(framepass%2 == 0)
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		process(**it, globalTime, time);
	else // reverse order
		for(vector<PhysicInterface*>::reverse_iterator it = object.rbegin(); it != object.rend(); it++)
			process(**it, globalTime, time);


/*	for(vector<Element>::iterator it = element.begin(); it != element.end(); it++)
	{
		Element &el = *it;
		PhysicInterface &o = *el.object;

	}
	}*/
	return true;
}

void PhysicEngine3D::incCollision(const Collision collision)
{
	allCollisions[collision]++;
}

int PhysicEngine3D::findCollision(const collisionSet col, const Collision collision)
{
	std::map<Collision, int>::const_iterator it = col.find(collision);

	if(it == col.end()) 
	{
		Collision collision2 = collision;
		collision2.a = collision.b;
		collision2.b = collision.a;

		std::map<Collision, int>::const_iterator it2 = col.find(collision2);
		if(it2 == col.end())
			return false;
		else
			return (*it2).second;
	}
	else
		return (*it).second;
}


bool PhysicEngine3D::collisionReaction(Collision collision)
{
	ProcessKind::ProcessKind akind = collision.a->getProcessKind();
	ProcessKind::ProcessKind bkind = collision.b->getProcessKind();
	if(akind == ProcessKind::uni && bkind == ProcessKind::uni)
		return collisionReactionUniUni(collision); 

	if(akind == ProcessKind::uni && (bkind == ProcessKind::none || bkind == ProcessKind::custom))
		return collisionReactionUniNone(collision);

	if(bkind == ProcessKind::uni && (akind == ProcessKind::none || akind == ProcessKind::custom))
	{
		PhysicInterface *swap = collision.a;
		collision.a = collision.b; 
		collision.b = swap;

		return collisionReactionUniNone(collision);
	}

	return false;
}

// distance between line and point
float getDist(Line line, v3 point) 
{
	v3 n = line.a.getNormalized();
	return (n*(point-line.base)).getLength();
}

v3	getSpeed(PhysicInterface &o, v3 point)
{
	v3 center = o.getGlobalMatrix()*v3(0,0,0);
	velocity v = o.getGlobalVelocity();
//	float dist = getDist(Line(center, v.rotationAxis), point);
	v3 rs = v.rotationAxis*(point - center);

	return v.translation + rs;
}


void PhysicEngine3D::applyImpule(PhysicInterface &a, v3 point, v3 impulse)
{
	v3 pc = point - a.getGlobalMatrix()*v3(0,0,0);// point to center
	v3 pcn = pc.getNormalized();
	float m = a.getMass();
	velocity v = a.getGlobalVelocity();
	v.translation += (pcn&impulse)*pcn;
	a.setVelocity(v);
}


bool PhysicEngine3D::collisionReactionUniNone(const Collision collision)
{
	v3 t = collision.normal.getNormalized();

	velocity V1 = collision.a->getVelocity(); 
	v3 v1 = getSpeed(*collision.a, collision.point);
	Config *aconf = collision.a->getPMaterial();
	Config *bconf = collision.b->getPMaterial();

	velocity V2 = collision.b->getVelocity(); 
//	v3 v2 = V2.translation;
	v3 v2 = getSpeed(*collision.b, collision.point);

// энергия
	float rk = 0;
	
	float Tmin = v2.getSquaredLength() + rk*V2.rotationAxis.getSquaredLength();
	float Tmax = v1.getSquaredLength() + rk*V1.rotationAxis.getSquaredLength();

	float energySave = clamp(1 - aconf->getf("energyLose",0) + bconf->getf("energyLose",0));

	float energyConstLose = aconf->getf("energyConstLose",0) + bconf->getf("energyConstLose",0);
	energyConstLose *= g.getLength();

	float T = Tmin*(1-energySave) + Tmax*energySave - energyConstLose;
	if(T<Tmin) T = Tmin;
	float Td = Tmax - Tmin;

/*	float min_energy = 10;
	if(T - Tmin < min_energy) T = Tmin;*/

	float v1t = t&v1; float v2t = t&v2;

	v3 v1sr = v1 - t*v1t;
	v3 v2sr = v2 - t*v2t;

	v3 v12sr = v1sr - v2sr; // относительная скорость в точке удара

	float friction = 1 - clamp(aconf->getf("friction", 0.0f) + bconf->getf("friction", 0.0f));
	float min_friction = aconf->getf("min_friction", 0.0f) + bconf->getf("min_friction", 0.0f);

	v12sr *= friction;
	if(v12sr.getLength()<min_friction) v12sr.loadZero();

	v1sr = v2sr + v12sr;

	float u1t = v2t - v1t; 
//	float u1t = T - v1sr.getSquaredLength();
	v3 u1a = v1sr + t*u1t; // линейная скорость в точке удара
	v3 u1b = v2;

	u1a.x = u1b.x*(1-energySave) + u1a.x*energySave;
	u1a.y = u1b.y*(1-energySave) + u1a.y*energySave;
	u1a.z = u1a.z*((T-Tmin)/Td) + u1b.z*((Tmax-T)/Td);

	if(conf->geti("crRotation"))
	{
/*		v3 center = collision.a->getGlobalMatrix()*v3(0,0,0);

		float center_point = (center - collision.point).getLength();
		
		v3 normal = ((center - collision.point)*v3(0,0,1)).getNormalized()*center_point;

		v3 pa = collision.point;
		v3 pb = center + (center - collision.point)*0.5f + normal*sqrt(3.0f)/2;
		v3 pc = center + (center - collision.point)*0.5f - normal*sqrt(3.0f)/2;

		v3 u1b = getSpeed(*collision.a, pb); // линейная скорость в противоположной точке
		v3 u1c = getSpeed(*collision.a, pc); // линейная скорость в противоположной точке

		helper->drawLine(Line(pa, pb-pa), 0,0, v4());
		helper->drawLine(Line(pa, pc-pa), 0,0, v4());
		helper->drawLine(Line(pb, pc-pb), 0,0, v4());

		helper->drawVector(Line(pa, u1a*0.1f), 0,0, v4());
		helper->drawVector(Line(pb, u1b*0.1f), 0,0, v4());
		helper->drawVector(Line(pc, u1c*0.1f), 0,0, v4());

		v3 da = (center-pa).getNormalized();
		v3 db = (center-pb).getNormalized();
		v3 dc = (center-pc).getNormalized();


		v3 rot = ((u1a*da) + (u1b*db) + (u1c*dc)).getNormalized();

		v3 tr = (u1a&da)*da;
		
		T -= tr.getSquaredLength();
		if(T>EPSILON)
			rot *= sqrt(T/rk);
		else
			rot.loadZero();

		V1.translation = tr;
		V1.rotationAxis= rot;*/
	}
	else
	{
		V1.translation = u1a;
	}
	collision.a->setVelocity(V1);

	return true;
}

bool PhysicEngine3D::collisionReactionUniUni(const Collision collision)
{ // Only global
	float m1 = collision.a->getMass();
	float m2 = collision.b->getMass();

	v3 t = collision.normal.getNormalized();

	velocity V1 = collision.a->getVelocity(); v3 v1 = V1.translation; float v1t = t&v1;	
	velocity V2 = collision.b->getVelocity(); v3 v2 = V2.translation; float v2t = t&v2;

	Config *aconf = collision.a->getPMaterial();
	Config *bconf = collision.b->getPMaterial();

	float friction = 1 - clamp(aconf->getf("friction", 0.0f) + bconf->getf("friction", 0.0f));
	float min_friction = aconf->getf("min_friction", 0.0f) + bconf->getf("min_friction", 0.0f);

	float energySave = clamp(1 - aconf->getf("energyLose",0) - bconf->getf("energyLose",0));

//	if(v1t*v2t>0) return false;

	float It = v1t*m1 + v2t*m2;

	float energyConstLose = aconf->getf("energyConstLose",0) + bconf->getf("energyConstLose",0);
	energyConstLose *= g.getLength();

	float E = m1*v1.getSquaredLength() + m2*v2.getSquaredLength() - energyConstLose;
//	E *= 0.5;
	float V1sr2 = v1.getSquaredLength() - v1t*v1t; if(V1sr2<EPSILON) V1sr2 = 0;
	float V2sr2 = v2.getSquaredLength() - v2t*v2t; if(V2sr2<EPSILON) V2sr2 = 0;

	float V1sr = sqrt(V1sr2);
	float V2sr = sqrt(V2sr2);
	float k1 = m1/(m1+m2);
	float k2 = m2/(m1+m2);

	float Vsr = k1*V1sr +k2*V2sr;

	V1sr = Vsr +  (V1sr - Vsr)*friction*k2 + (V1sr - Vsr)*k1;
	V2sr = Vsr +  (V2sr - Vsr)*friction*k1 + (V2sr - Vsr)*k2;

//	v12sr *= friction;
//	if(v12sr.getLength()<min_friction) v12sr.loadZero();

	// квадратное уравнение (16)
	float A = m2*m2/m1 + m2;
	float B = -2*It*m2/m1;
	float C = It*It/m1 + m1*V1sr*V1sr + m2*V2sr*V2sr - E;

	float D = B*B - 4*A*C;

	D *= energySave;

	if (D<EPSILON) D = 0;//	assert(D>=0, "collisionReaction D>=0");

	float u2ta = (-B + sqrt(D))/(2*A);	float u1ta = (It - m2*u2ta)/m1; 
	float u2tb = (-B - sqrt(D))/(2*A);	float u1tb = (It - m2*u2tb)/m1;

	float u1t, u2t;

	if(sgn(v1t - v2t) != sgn(u1ta - u2ta))
		u1t = u1ta,	u2t = u2ta;
	else
		u1t = u1tb,	u2t = u2tb;
	
	v3 v1sr = (v1 - t*v1t).getNormalized()*V1sr;
	v3 v2sr = (v2 - t*v2t).getNormalized()*V2sr;
	v3 u1 = v1sr + t*u1t;
	v3 u2 = v2sr + t*u2t;

	V1.translation = u1;	collision.a->setVelocity(V1);
	V2.translation = u2;	collision.b->setVelocity(V2);

	return true;
}

// Check for collision между одним объектом и другим (и всеми его детьми)
// родитель не сталкивается с детьми
void PhysicEngine3D::collisionDetection(PhysicInterface &a, PhysicInterface &b, v3 distance, Collision &collision, PhysicInterface *clip)
{
	if(&a == &b || &b == clip) return ;
			
	if(b.getTriangles() && b.getPVertexes()) 
	{	
		Collision newCollision;
		newCollision.time = INF;
		if(checkCollision(a, distance, b, newCollision) && collision.time > newCollision.time)
				collision = newCollision, collision.b = &b;
	}

	PhysicInterfaceList c = b.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		collisionDetection(a, **it, distance, collision, clip);
}

void PhysicEngine3D::collisionDetectionRotation(PhysicInterface &a, PhysicInterface &b, const matrix44 rotation, Collision &collision, PhysicInterface *clip)
{
	if(&a == &b || &b == clip) return ;
			
	if(b.getTriangles() && b.getPVertexes()) 
	{	
		Collision newCollision;
		newCollision.time = INF;
		if(checkCollisionRotation(a, rotation, b, newCollision) && collision.time > newCollision.time)
				collision = newCollision, collision.b = &b;
	}

	PhysicInterfaceList c = b.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		collisionDetectionRotation(a, **it, rotation, collision, clip);
}


// Check for collision между одним объектом и всеми
void PhysicEngine3D::collisionDetection(PhysicInterface &o, v3 distance, Collision &collision, PhysicInterface *clip)
{
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		if((*it) != &o)
			collisionDetection(o, **it, distance, collision, clip);

	PhysicInterfaceList c = o.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		collisionDetection(**it, distance, collision, clip);
}

// Check for collision между одним объектом и всеми
void PhysicEngine3D::collisionDetectionRotation(PhysicInterface &o, const matrix44 rotation, Collision &collision, PhysicInterface *clip)
{
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		if((*it) != &o)
			collisionDetectionRotation(o, **it, rotation, collision, clip);

	PhysicInterfaceList c = o.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		collisionDetectionRotation(**it, rotation, collision, clip);
}


// пересечение отрезка (движущейся точки) и треугольника
void PhysicEngine3D::checkCollisionMVertexTrg(const v3 point, const v3 direction, const Plane b, Collision &collision)
{
	float k;

	if(isCross(b, Line(point, direction), k))
	{
		if(k>=-EPSILON && k<=1+EPSILON)
		{
			v3 cpoint = point + direction*k;

			if(b.isInTrg(cpoint) && collision.time > k)
			{
				collision.time = k;
				collision.point = cpoint;
				collision.normal = b.a*b.b; //нормаль к треугольнику
			}
		}
	}
}

void PhysicEngine3D::checkCollisionMTrgVertex(const Plane a, const v3 direction, const v3 point, Collision &collision)
{
	if(pointInPrism(a, direction, point))
	{
		float newTime = 0.5;
		
		bool cross = isCross(a, Line(point, -direction), newTime);
//		assert(cross, "checkCollisionMTrgVertex");

		if(cross && collision.time > newTime)
		{
			collision.time = newTime;
			collision.point = point;
			collision.normal = a.a*a.b; //нормаль к треугольнику
		}
//		isCross(a, Line(point, -direction), newTime);
	}
}


void PhysicEngine3D::checkCollisionRTrgVertex1up(const Plane a, v3 d1, v3 d2, v3 d3, const v3 point, Collision &collision)
{
	if(
		byRightSide(point, a)
		&& byRightSide(point, Plane(a.base + d1, a.b + d3- d1, a.a + d2 - d1))
		&& byRightSide(point, Plane(a.base, d1, a.a))
		&& byRightSide(point, Plane(a.base, a.b, d1))
		)
	{
			collision.time = 0;
			collision.point = point;
			collision.normal = a.a*a.b; // TODO
	}
}

void PhysicEngine3D::checkCollisionRTrgVertex2up(const Plane a, v3 d1, v3 d2, v3 d3, const v3 point, Collision &collision)
{
	if(
		   byRightSide(point, a)
		&& byRightSide(point, Plane(a.base + d1, a.b + d3- d1, a.a + d2 - d1))
		&& byRightSide(point, Plane(a.base, d1, a.a))
		&& byRightSide(point, Plane(a.base, a.b, d1))
		&& byRightSide(point, Plane(a.base + a.a, d2, a.b-a.a))
		)
	{
			collision.time = 0;
			collision.point = point;
			collision.normal = a.a*a.b; // TODO
	}
}

void PhysicEngine3D::checkCollisionRTrgVertex3up(const Plane a, v3 d1, v3 d2, v3 d3, const v3 point, Collision &collision)
{
	if(
		   byRightSide(point, a)
		&& byRightSide(point, Plane(a.base + d1, a.b + d3- d1, a.a + d2 - d1))
		&& byRightSide(point, Plane(a.base, d1, a.a))
		&& byRightSide(point, Plane(a.base, a.b, d1))
		&& byRightSide(point, Plane(a.base + a.a, d2, a.b-a.a))
		)
	{
			collision.time = 0;
			collision.point = point;
			collision.normal = a.a*a.b; // TODO
	}
}


void PhysicEngine3D::checkCollisionMLineLine(const Line a, const v3 direction, const Line b, Collision &collision)
{
	float k;
	if(crossMLineLine(a, direction, b, k))
	{
		v3 point = b.point(k);
		float time;
		bool c = isCrossFast(a, Line(point, -direction), time);
		assert(c, "checkCollisionMLineLine");
		
		if(collision.time > time /*+ 1.0e-4*/)
		{
			collision.time = time;
			collision.point = point - direction * time;
			collision.normal = a.a*b.a;
		}
	}
}

void PhysicEngine3D::checkCollisionRLineLine2up(const Line a,	v3 d1, v3 d2, const Line b,	Collision &collision)
{
	float k;

	if(((a.base*a.a)&d1)<EPSILON) 
		return;

	if(!isCross(Plane(a.base, a.a, d1), b, k)) return ;
	if(k<-EPSILON || k>1+ EPSILON) return ;

	v3 point = b.point(k);

	if(	isBetween(point, a.base, a.a, d1) &&	
		isBetween(point, a.base + a.a + d2, d2 + a.a - d1,-d2))
	{
		collision.time = 0;
		collision.point = point;
		collision.normal = a.a*b.a;
	}
}

void PhysicEngine3D::checkCollisionRLineLine1up(const Line a,	v3 d1, v3 d2, const Line b,	Collision &collision)
{
	float k;

	if(!isCross(Plane(a.base, a.a, d1), b, k)) return ;
	if(k<-EPSILON || k>1+ EPSILON) return ;

	v3 point = b.point(k);

	if(	isBetween(point, a.base, a.a, d1) &&	
		isBetween(point, a.base + d1, a.a + d2 - d1, -d1))
	{
		collision.time = 0;
		collision.point = point;
		collision.normal = a.a*b.a;
	}
}


void PhysicEngine3D::checkCollisionMLineTrg(const Line a, const v3 direction, const Plane b, Collision &collision)
{
	checkCollisionMLineLine(a, direction, Line(b.base, b.a), collision);
	checkCollisionMLineLine(a, direction, Line(b.base, b.b), collision);
	checkCollisionMLineLine(a, direction, Line(b.base + b.a, b.b-b.a), collision);
}

void PhysicEngine3D::checkCollisionRLineTrg1up(const Line a, v3 d1, v3 d2, const Plane b,	Collision &collision)
{
	checkCollisionRLineLine1up(a, d1, d2, Line(b.base, b.a), collision);
	checkCollisionRLineLine1up(a, d1, d2, Line(b.base, b.b), collision);
	checkCollisionRLineLine1up(a, d1, d2, Line(b.base + b.a, b.b-b.a), collision);
}

void PhysicEngine3D::checkCollisionRLineTrg2up(const Line a, v3 d1, v3 d2, const Plane b,	Collision &collision)
{
	checkCollisionRLineLine2up(a, d1, d2, Line(b.base, b.a), collision);
	checkCollisionRLineLine2up(a, d1, d2, Line(b.base, b.b), collision);
	checkCollisionRLineLine2up(a, d1, d2, Line(b.base + b.a, b.b-b.a), collision);
}

// пересечение движущегося треугольника (призма) с треугольником
// 1. вершина - грань
// 2. грань - вершина
// 3. ребро - ребро
bool PhysicEngine3D::checkCollisionMTrgTrg(Plane a, v3 direction, Plane b, Collision &collision, PhysicInterface &aobj, PhysicInterface &bobj)
{
	Collision start = collision;

// 1. вершина - грань
	checkCollisionMVertexTrg(a.base      , direction, b, collision);
	checkCollisionMVertexTrg(a.base + a.a, direction, b, collision);
	checkCollisionMVertexTrg(a.base + a.b, direction, b, collision);

// 2. грань - вершина
// точки b.base, b.base + b.a, b.base + b.b
/*	checkCollisionMVertexTrg(b.base, -direction, a, collision);
	checkCollisionMVertexTrg(b.base + b.a, -direction, a, collision);
	checkCollisionMVertexTrg(b.base + b.b, -direction, a, collision);*/

	checkCollisionMTrgVertex(a, direction, b.base, collision);
	checkCollisionMTrgVertex(a, direction, b.base + b.a, collision);
	checkCollisionMTrgVertex(a, direction, b.base + b.b, collision);

// 3. ребро - ребро
	checkCollisionMLineTrg(Line(a.base, a.a), direction, b, collision);
	checkCollisionMLineTrg(Line(a.base, a.b), direction, b, collision);
	checkCollisionMLineTrg(Line(a.base + a.a, a.b - a.a), direction, b, collision);

	if(start.time > collision.time)
	{
		v3 relativeVelocity = aobj.getGlobalVelocity().translation - bobj.getGlobalVelocity().translation;

		float n = relativeVelocity&(a.a*a.b);
		collision.fake = collision.fake && (n<0);
/*		if(n<0) 
			collision.fake = true;
		else
			collision.fake = false;*/
	}

	return collision.time<=1;
}


// пересечение врашающегося треугольника с треугольником
// 1. вершина - грань
// 2. грань - вершина
// 3. ребро - ребро
bool PhysicEngine3D::checkCollisionRTrgTrg(Plane a, v3 d1, v3 d2, v3 d3, Plane b, Collision &collision)
{
	v3 normal = a.a*a.b; int cnt = 0;
	bool u1 = (normal&d1)>0; if(u1) cnt++;
	bool u2 = (normal&d2)>0; if(u2) cnt++;
	bool u3 = (normal&d3)>0; if(u3) cnt++;

// 1. вершина - грань
	if(u1) checkCollisionMVertexTrg(a.base      , d1, b, collision);
	if(u2) checkCollisionMVertexTrg(a.base + a.a, d2, b, collision);
	if(u3) checkCollisionMVertexTrg(a.base + a.b, d3, b, collision);

	// 2. грань - вершина
	Plane na;
	v3 nd1, nd2, nd3;
	if(cnt==1)
	{
		if(u1)	na = a, nd1 = d1, nd2 = d2, nd3 = d3;
		if(u2) 
		{
			na.base = a.base + a.a;
			na.a = a.b - a.a;
			na.b = -a.a;
			nd1 = d2; 
			nd2 = d3;
			nd3 = d1;
		}
		if(u3) 
		{
			na.base = a.base + a.b;
			na.a = -a.b;
			na.b = a.a - a.b;
			nd1 = d3;
			nd2 = d1;
			nd3 = d2;
		}
		checkCollisionRTrgVertex1up(na, nd1, nd2, nd3, b.base, collision);
		checkCollisionRTrgVertex1up(na, nd1, nd2, nd3, b.base + b.a, collision);
		checkCollisionRTrgVertex1up(na, nd1, nd2, nd3, b.base + b.b, collision);
	}

	if(cnt==2)
	{
		if(!u3)	na = a, nd1 = d1, nd2 = d2, nd3 = d3;
		if(!u2)
		{
			na.base = a.base + a.b;
			na.a = -a.b;
			na.b = a.a - a.b;
			nd1 = d3;
			nd2 = d1;
			nd3 = d2;
		}
		if(u3) 
		{
			na.base = a.base + a.a;
			na.a = a.b - a.a;
			na.b = -a.a;
			nd1 = d2; 
			nd2 = d3;
			nd3 = d1;
		}
		checkCollisionRTrgVertex2up(na, nd1, nd2, nd3, b.base, collision);
		checkCollisionRTrgVertex2up(na, nd1, nd2, nd3, b.base + b.a, collision);
		checkCollisionRTrgVertex2up(na, nd1, nd2, nd3, b.base + b.b, collision);
	}

	if(cnt==3)
	{
		checkCollisionRTrgVertex3up(a, d1, d2, d3, b.base, collision);
		checkCollisionRTrgVertex3up(a, d1, d2, d3, b.base + b.a, collision);
		checkCollisionRTrgVertex3up(a, d1, d2, d3, b.base + b.b, collision);
	}

	// 3. ребро - ребро
/*	if(u1 && u2) checkCollisionRLineTrg2up(Line(a.base, a.a), d1, d2, b, collision);
	if(u1 && !u2) checkCollisionRLineTrg1up(Line(a.base, a.a), d1, d2, b, collision);
	if(!u1 && u2) checkCollisionRLineTrg1up(Line(a.base + a.a, -a.a), d2, d1, b, collision);

	if(u1 && u3) checkCollisionRLineTrg2up(Line(a.base, a.b), d1, d3, b, collision);
	if(u1 && !u3) checkCollisionRLineTrg1up(Line(a.base, a.b), d1, d3, b, collision);
	if(!u1 && u3) checkCollisionRLineTrg1up(Line(a.base + a.b, -a.b), d3, d1, b, collision);

	if(u2 && u3) checkCollisionRLineTrg2up(Line(a.base + a.a, a.b - a.a), d2, d3, b, collision);
	if(u2 && !u3) checkCollisionRLineTrg1up(Line(a.base + a.a, a.b - a.a), d2, d3, b, collision);
	if(!u2 && u3) checkCollisionRLineTrg1up(Line(a.base + a.b, -a.b + a.a), d3, d2, b, collision);
*/

	Plane up(a.base + d1, a.a + d2 - d1, a.b + d3 - d1);
	checkCollisionMVertexTrg(b.base, b.a, up, collision);
	checkCollisionMVertexTrg(b.base, b.b, up, collision);
	checkCollisionMVertexTrg(b.base + b.a, b.b - b.a, up, collision);

	if(collision.time < 2) collision.time = 0;

//	checkCollisionRLineTrg2up(Line(a.base + a.a, a.b - a.a), d2, d3, b, collision);

/*	aabb abox(a.base); abox.merge(a.base + a.a); abox.merge(a.base + a.b);
	aabb anewbox(anew.base); anewbox.merge(anew.base + anew.a); anewbox.merge(anew.base + anew.b);
	aabb bbox(b.base); bbox.merge(b.base + b.a); bbox.merge(b.base + b.b);

	abox.merge(anewbox);

//	if(abox.intersect(bbox))
	{
		helper->drawBox(abox, 0,0,v4());
		helper->drawBox(bbox, 0,0,v4());

//		collision.time = 0;
//		return true;
	}*/

	return collision.time<=1;
}


// проверяет коллизию между движушимся телом а
// и неподвижным телом b. Тело а движется на расстояние distance
// функция возвращает самую первую во ремени коллизию
bool PhysicEngine3D::checkCollision(PhysicInterface &a, v3 distance, PhysicInterface &b, Collision &collision)
{
	// каждый треугольник движущегося тела при движении образует призму
	// проверяем пересечение этой призмы со всеми треугольниками второго тела
	aabb newframe1 = a.getPFrame();	newframe1.mul(a.getGlobalMatrix());	newframe1.add(distance);
	aabb newframe2 = b.getPFrame();	newframe2.mul(b.getGlobalMatrix());

	newframe1.cross(newframe2);
	if(newframe1.empty()) return false;

	Triangles *t = a.getTriangles();	Vertexes *v = a.getPVertexes();		matrix44 matrix = a.getGlobalMatrix();
	Triangles *t2 = b.getTriangles();	Vertexes *v2 = b.getPVertexes();	matrix44 matrix2 = b.getGlobalMatrix();

	vector<Plane> bp;
	for(vector<Triangle>::iterator jt = t2->data.begin(); jt != t2->data.end(); jt++)
	{
		Plane bt;
		bt.base = matrix2 * v2->data[jt->a[0]];
		bt.a	= matrix2 * v2->data[jt->a[1]] - bt.base;
		bt.b	= matrix2 * v2->data[jt->a[2]] - bt.base;
		aabb aabb2(bt.base);		aabb2.merge(bt.base + bt.a);		aabb2.merge(bt.base + bt.b);

		if(newframe1.intersect(aabb2))
			bp.push_back(bt);
	}

	Plane at;
	if(t)
	for(vector<Triangle>::iterator it = t->data.begin(); it != t->data.end(); it++)
	{
		at.base = matrix * v->data[it->a[0]];
		at.a	= matrix * v->data[it->a[1]] - at.base;
		at.b	= matrix * v->data[it->a[2]] - at.base;
		if( ((at.a*at.b) & distance) > 0) // треугольник движется вперёд
		{
			aabb aabb1(at.base, at.base);
			aabb1.add(at.a);
			aabb1.add(at.b);
			aabb1.add(distance);

			bool ok = false;
			if(newframe1.intersect(aabb1))
			{
				for(vector<Plane>::iterator jt = bp.begin(); jt != bp.end(); jt++)
				{
					Plane bt = *jt;

					Collision newCollision;	newCollision.time = INF;
					if(checkCollisionMTrgTrg(at, distance, bt, newCollision, a, b) && collision.time > newCollision.time)
					{
						helper->drawLine(Line(at.base + distance*collision.time, at.a), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));
						helper->drawLine(Line(at.base + distance*collision.time, at.b), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));
						helper->drawLine(Line(at.base + at.a + distance*collision.time, at.b - at.a), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));

						helper->drawLine(Line(bt.base, bt.a), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));
						helper->drawLine(Line(bt.base, bt.b), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));
						helper->drawLine(Line(bt.base + bt.a, bt.b - bt.a), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));

						collision = newCollision;
						ok = true;
					}
				}
			}
		}
	}
	return collision.time<=1;
}


// проверяет коллизию между врашающимя телом а
// и неподвижным телом b. 
// функция возвращает самую первую во ремени коллизию
bool PhysicEngine3D::checkCollisionRotation(PhysicInterface &a, const matrix44 rot, PhysicInterface &b, Collision &collision)
{
	// каждый треугольник движущегося тела при движении образует призму
	// проверяем пересечение этой призмы со всеми треугольниками второго тела
	aabb newframe1(a.getPFrame());		newframe1.mul(a.getGlobalMatrix());
	aabb newframe1b(newframe1);	newframe1.mul(rot);	
	newframe1.merge(newframe1b);

	aabb newframe2 = b.getPFrame();	newframe2.mul(b.getGlobalMatrix());

	newframe1.cross(newframe2);
	if(newframe1.empty()) return false;

	Triangles *t = a.getTriangles();	Vertexes *v = a.getPVertexes();		matrix44 matrix = a.getGlobalMatrix();
	Triangles *t2 = b.getTriangles();	Vertexes *v2 = b.getPVertexes();	matrix44 matrix2 = b.getGlobalMatrix();

	if(!t2 || !t) return false;
	vector<Plane> bp;
	for(vector<Triangle>::iterator jt = t2->data.begin(); jt != t2->data.end(); jt++)
	{
		Plane bt;	bt.base = matrix2 * v2->data[jt->a[0]];		bt.a	= matrix2 * v2->data[jt->a[1]] - bt.base;		bt.b	= matrix2 * v2->data[jt->a[2]] - bt.base;
		aabb aabb2(bt.base);		aabb2.merge(bt.base + bt.a);		aabb2.merge(bt.base + bt.b);
		if(newframe1.intersect(aabb2))	
			bp.push_back(bt);
	}

	Plane at, anew;
	matrix44 newpos = matrix*rot;

	for(vector<Triangle>::iterator it = t->data.begin(); it != t->data.end(); it++)
	{
		at.base = matrix * v->data[it->a[0]];
		at.a	= matrix * v->data[it->a[1]] - at.base;
		at.b	= matrix * v->data[it->a[2]] - at.base;

		aabb old(at.base); old.merge(at.base + at.a); old.merge(at.base + at.b);

		anew.base = newpos* v->data[it->a[0]];
		anew.a	= newpos * v->data[it->a[1]] - anew.base;
		anew.b	= newpos * v->data[it->a[2]] - anew.base;

		v3 d1 = anew.base - at.base;
		v3 d2 = anew.base + anew.a - (at.base + at.a);
		v3 d3 = anew.base + anew.b - (at.base + at.b);

		old.merge(anew.base);	old.merge(anew.base + anew.a); old.merge(anew.base + anew.b);

		v3 normal = at.a*at.b;

		if( 
			(normal & d1) > 0 ||
			(normal & d2) > 0 ||
			(normal & d3) > 0
		)
			
			// треугольник движется вперёд
		{

			bool ok = false;
			if(newframe1.intersect(old))
			{
				for(vector<Plane>::iterator jt = bp.begin(); jt != bp.end(); jt++)
				{
					Plane bt = *jt;

					Collision newCollision;	newCollision.time = INF;
					if(checkCollisionRTrgTrg(at, d1, d2, d3, bt, newCollision) && collision.time > newCollision.time)
							collision = newCollision, ok = true;
				}
			}
		}
	}
	return collision.time<=1;
}


bool PhysicEngine3D::update(Element &el)
{
	Interface::PositionKind pos = el.object->getPositionKind();

	el.matrix   = el.object->getMatrix();
	el.frame	= el.object->getPFrame(); // local
	if(pos == Interface::local)
		el.matrix = el.parentMatrix*el.matrix; // TODO: update parent matrix
	
	el.frame.mul(el.matrix); // global

	el.frame.min -= v3(CONTACT_EPSILON, CONTACT_EPSILON, CONTACT_EPSILON);
	el.frame.max += v3(CONTACT_EPSILON, CONTACT_EPSILON, CONTACT_EPSILON);

	return true;
}


bool PhysicEngine3D::prepare(PhysicInterface *object, matrix44 parent_matrix, PhysicInterface *parent)
{
	if(!object) return false;

	PhysicInterfaceList children = object->getPChildrens();
	for(PhysicInterfaceList::iterator it=children.begin();	it != children.end(); it++)
		if(!prepare(*it)) return false;

/*	Interface::PositionKind pos = object->getPositionKind();
	ProcessKind::ProcessKind process = object->getProcessKind();
	if(process == ProcessKind::uni && pos == Interface::local)
	{
		object->changePositionKind(Interface::global);
		pos = object->getPositionKind();
	}
*/
/*	el.object	= object;
	el.parent	= parent;
	el.vertex	= object->getPVertexes();
	el.triangle = object->getTriangles();
	el.frame	= object->getPFrame(); // local
	el.matrix   = object->getMatrix();
	el.collisionCount = 0;

	if(pos == Interface::local)
		el.matrix = parent_matrix*el.matrix;

	el.frame.mul(el.matrix); // global

	el.frame.min -= v3(CONTACT_EPSILON, CONTACT_EPSILON, CONTACT_EPSILON);
	el.frame.max += v3(CONTACT_EPSILON, CONTACT_EPSILON, CONTACT_EPSILON);

	element.push_back(el);
	PhysicInterface &o = *(PhysicInterface*)object;
	PhysicInterfaceList children = o.getPChildrens();
	for(PhysicInterfaceList::iterator it=children.begin();	it != children.end(); it++)
		if(!prepare(*it, el.matrix, object)) return false;
*/
/*	for(FaceMaterials::iterator it = m->begin(); it != m->end(); it++)
	{
2D hashing
	}
*/
	return true;
}



bool PhysicEngine3D::init(std::string _conf)
{
	if(!PhysicEngine::init(_conf)) return false;

	g.z = -(float)conf->getf("g", 0.0);
	g.x = 0.0f;
	g.y = 0.0f;

	return true;
}

bool PhysicEngine3D::clear()
{
	if(!PhysicEngine::clear()) return false;
//	tag.clear();
	return true;
}

bool PhysicEngine3D::checkInvariant(PhysicInterface &o, PhysicInterface &clip)
{
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		if((*it) != &o)
			if(!checkInvariant(o, **it, clip)) return false;

	PhysicInterfaceList c = o.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		if(!checkInvariant(**it, clip)) return false;
	return true;
}

bool PhysicEngine3D::checkInvariant(PhysicInterface &a, PhysicInterface &b, PhysicInterface &clip)
{
	if(&a == &b || &b == &clip) return true;
			
	if(b.getTriangles() && b.getPVertexes()) 
	{	
		// a cross b ?
		if(crossModelModel(a, b)) return false;
	}

	PhysicInterfaceList c = b.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		if(!checkInvariant(a, **it, clip)) return false;
	return true;
}

// пересекаются или находятся внутри
bool PhysicEngine3D::crossModelModel(PhysicInterface &a, PhysicInterface &b)
{
	Vertexes *v1 = a.getPVertexes();
	Vertexes *v2 = b.getPVertexes();
	if(!v1 || !v2 || v1->data.empty() || v2->data.empty()) return false;

	if(intersectModelModel(a,b)) return true;

	if(crossPointModel(a.getGlobalMatrix()*v1->data[0], b)) return true;

	if(crossPointModel(b.getGlobalMatrix()*v2->data[0], a)) return true;
	return false;
}

// пересекаются контуры
bool PhysicEngine3D::intersectModelModel(PhysicInterface &a, PhysicInterface &b)
{
	// каждый треугольник движущегося тела при движении образует призму
	// проверяем пересечение этой призмы со всеми треугольниками второго тела
	aabb newframe1 = a.getPFrame();	newframe1.mul(a.getGlobalMatrix());
	aabb newframe2 = b.getPFrame();	newframe2.mul(b.getGlobalMatrix());

	newframe1.cross(newframe2);
	if(newframe1.empty()) return false;

	Triangles *t = a.getTriangles();	Vertexes *v = a.getPVertexes();		matrix44 matrix = a.getGlobalMatrix();
	Triangles *t2 = b.getTriangles();	Vertexes *v2 = b.getPVertexes();	matrix44 matrix2 = b.getGlobalMatrix();

	vector<Plane> bp;
	for(vector<Triangle>::iterator jt = t2->data.begin(); jt != t2->data.end(); jt++)
	{
		Plane bt;
		bt.base = matrix2 * v2->data[jt->a[0]];
		bt.a	= matrix2 * v2->data[jt->a[1]] - bt.base;
		bt.b	= matrix2 * v2->data[jt->a[2]] - bt.base;
		aabb aabb2(bt.base);		aabb2.merge(bt.base + bt.a);		aabb2.merge(bt.base + bt.b);

		if(newframe1.intersect(aabb2))
			bp.push_back(bt);
	}

	Plane at;
	if(t)
	for(vector<Triangle>::iterator it = t->data.begin(); it != t->data.end(); it++)
	{
		at.base = matrix * v->data[it->a[0]];
		at.a	= matrix * v->data[it->a[1]] - at.base;
		at.b	= matrix * v->data[it->a[2]] - at.base;

		aabb aabb1(at.base, at.base);
		aabb1.add(at.a);
		aabb1.add(at.b);

		bool ok = false;
		if(newframe1.intersect(aabb1))
		{
			for(vector<Plane>::iterator jt = bp.begin(); jt != bp.end(); jt++)
			{
				Plane bt = *jt;

				if(intersectTrgTrg(at, bt)) return true;
			}
		}
	}
	return false;
}

bool PhysicEngine3D::intersectTrgTrg(Plane a, Plane b)
{
	return 
		intersectLineTrg(Line(a.base, a.a), b) ||
		intersectLineTrg(Line(a.base, a.b), b) ||
		intersectLineTrg(Line(a.base + a.a, a.b - a.a), b);
}

bool PhysicEngine3D::intersectLineTrg(Line a, Plane b)
{
	float k;
	if(isCross(b, a, k))
	{
		v3 cpoint = a.point(k);
		return (k>-EPSILON && k<1+EPSILON && b.isInTrg(cpoint));
	}
	return false;
}


// точка находится внутри
bool PhysicEngine3D::crossPointModel(v3 point, PhysicInterface &a)
{
	Triangles *t = a.getTriangles();	
	Vertexes *v = a.getPVertexes();		
	matrix44 matrix = a.getGlobalMatrix();

	int cnt = 0;

	Plane at;
	if(t)
	for(vector<Triangle>::iterator it = t->data.begin(); it != t->data.end(); it++)
	{
		at.base = matrix * v->data[it->a[0]];
		at.a	= matrix * v->data[it->a[1]] - at.base;
		at.b	= matrix * v->data[it->a[2]] - at.base;

		float k;
		if(isCross(at, Line(point, v3(0,0,1)), k))
		{
			v3 cpoint = point + v3(0,0,1)*k;
			if(k>-EPSILON && at.isInTrg(cpoint))
				cnt++;
		}
	}
	if(cnt%2 == 0)
		return false;
	else
		return true;
}
