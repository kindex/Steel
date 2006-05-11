
#include "physic_engine_3d.h"

#include "../math/plane.h"

using namespace std;

#define CONTACT_EPSILON (0.001f)

bool PhysicEngine3D::process(steel::time globalTime, steel::time time)
{
	element.clear();
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		prepare(*it);

	v3 acc = g*(float)time;
	for(vector<Element>::iterator it = element.begin(); it != element.end(); it++)
	if(it->collisionCount == 0)
	{
		Element &el = *it;
		PhysicInterface &o = *el.object;

		ProcessKind::ProcessKind kind = o.getProcessKind();
		if(kind == ProcessKind::none) continue;

		if(kind == ProcessKind::custom)
		{
			o.process(globalTime, time, this);
		}
		if(kind == ProcessKind::uni)
		{
			v3 v = o.getVelocity();
			v += acc;	// gravitation
			o.setVelocity(v);
		}

		matrix44 &global = it->matrix;

		v3 velocity = o.getVelocity();

//		velocity = v3(0,0, -10);

		v3 path = velocity*(float)time;

		v3 oldPos = o.getPosition();
		v3 newPos = oldPos + path;

		if(helper) // draw velocity
			helper->drawVector(Line(global*oldPos ,velocity), 0,0);

		Collision collision;
		collision.time = INF; // no collision

		Interface::PositionKind pos = o.getPositionKind();
		v3 dir;

		if(pos == Interface::local)
			dir = global*newPos-global*oldPos;
		else
			dir = newPos - oldPos;

		if(collisionDetection(el, dir, collision))
		{
			float len = path.getLength();
			len *= collision.time;
			len -= CONTACT_EPSILON;
			if(len>0)
			{
				path = path.getNormalized()*len;
				newPos = oldPos + path;
				o.setPosition(newPos);
				update(el);
				//el.frame.min += path;
				//el.frame.max += path;
			}

			collision.a = &el;

			if(collision.b->collisionCount==0)
				collisionReaction(collision);

			collision.b->collisionCount++;
			el.collisionCount++;

			total.collisionCount++;
		}
		else
		{
			o.setPosition(newPos);

			update(el);
//			el.frame.min += path;
//			el.frame.max += path;
//			o.setPosition(p);
		}
	}
	return true;
}

bool PhysicEngine3D::collisionReaction(const Collision collision)
{ // Only global
	float m1 = collision.a->object->getMass();
	if(collision.a->object->getProcessKind() == ProcessKind::none) m1 = INF;
	float m2 = collision.b->object->getMass();
	if(collision.b->object->getProcessKind() == ProcessKind::none) m2 = INF;


	v3 t = collision.normal.getNormalized();
	v3 v1 = collision.a->object->getVelocity();
	v3 v2 = collision.b->object->getVelocity();
	float v1t = t&v1;
	float v2t = t&v2;

	float It = v1t*m1 + v2t*m2;

	float E = m1*v1.getSquaredLength() + m2*v2.getSquaredLength();
//	E *= 0.5;
	float V1sr2 = v1.getSquaredLength() - v1t*v1t; if(V1sr2<EPSILON) V1sr2 = 0;
	float V2sr2 = v2.getSquaredLength() - v2t*v2t; if(V2sr2<EPSILON) V2sr2 = 0;

// квадратное уравнение (16)
	float A = m2*m2/m1 + m2;
	float B = -2*It*m2/m1;
	float C = It*It/m1 + m1*V1sr2 + m2*V2sr2 - E;

	float D = B*B - 4*A*C;

	if (fabs(D)<EPSILON) D = 0;
	if (D<EPSILON) D = 0;

	assert(D>=0, "collisionReaction D>=0");

	float u2ta = (-B + sqrt(D))/(2*A);
	float u2tb = (-B - sqrt(D))/(2*A);
    float u1ta = (It - m2*u2ta)/m1;
    float u1tb = (It - m2*u2tb)/m1;

	float u1t, u2t;

	if(sgn(v1t - v2t) != sgn(u1ta - u2ta))
	{
		u1t = u1ta;
		u2t = u2ta;
	}
	else
	{
		u1t = u1tb;
		u2t = u2tb;
	}

	v3 v1sr = v1 - t*v1t;
	v3 v2sr = v2 - t*v2t;
	v3 u1 = v1sr + t*u1t;
	v3 u2 = v2sr + t*u2t;

//	v3 secondVelocity = collision.b->object->getVelocity();

	collision.a->object->setVelocity(u1);
	collision.b->object->setVelocity(u2);

	return true;
}


// Check for collision
bool PhysicEngine3D::collisionDetection(Element &a, v3 distance, Collision &collision)
{
	aabb newframe1 = a.frame;
	newframe1.add(distance);

	for(vector<Element>::iterator it = element.begin(); it != element.end(); it++)
		if(it->triangle && it->vertex && it->object != a.object)
		{
			Element &b = *it;
			
			if(newframe1.intersect(b.frame)) 
			{
				Collision newCollision;
				newCollision.time = INF;
				if(checkCollision(a, distance, b, newCollision))
				{
					if(collision.time > newCollision.time)
					{
						collision = newCollision;
						collision.b = &b;
					}
//					PElement &second
				}
			}
//			a.matrix
//			b.matrix
		}
	return collision.time<=1;
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
		
		pointInPrism(a, direction, point);

		bool cross = isCross(a, Line(point, -direction), newTime);
		
		assert(cross, "checkCollisionMTrgVertex");

		if(collision.time > newTime)
		{
			collision.time = newTime;
			collision.point = point;
			collision.normal = a.a*a.b; //нормаль к треугольнику
		}
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
		
		if(collision.time > time)
		{
			collision.time = time;
			collision.point = point;
			collision.normal = a.a*b.a;
		}
	}
}

void PhysicEngine3D::checkCollisionMLineTrg(const Line a, const v3 direction, const Plane b, Collision &collision)
{
	checkCollisionMLineLine(a, direction, Line(b.base, b.a), collision);
	checkCollisionMLineLine(a, direction, Line(b.base, b.b), collision);
	checkCollisionMLineLine(a, direction, Line(b.base + b.a, b.b-b.a), collision);
}


// пересечение движущегося треугольника (призма) с треугольником
// 1. вершина - грань
// 2. грань - вершина
// 3. грань - грань
bool PhysicEngine3D::checkCollisionMTrgTrg(Plane a, v3 direction, Plane b, Collision &collision)
{
// 1. вершина - грань
	checkCollisionMVertexTrg(a.base      , direction, b, collision);
	checkCollisionMVertexTrg(a.base + a.a, direction, b, collision);
	checkCollisionMVertexTrg(a.base + a.b, direction, b, collision);

// 2. грань - вершина
// точки b.base, b.base + b.a, b.base + b.b
	checkCollisionMTrgVertex(a, direction, b.base, collision);
	checkCollisionMTrgVertex(a, direction, b.base + b.a, collision);
	checkCollisionMTrgVertex(a, direction, b.base + b.b, collision);

// 3. грань - грань
	checkCollisionMLineTrg(Line(a.base, a.a), direction, b, collision);
	checkCollisionMLineTrg(Line(a.base, a.b), direction, b, collision);
	checkCollisionMLineTrg(Line(a.base + a.a, a.b - a.a), direction, b, collision);

	return collision.time<=1;
}


// проверяет коллизию между движушимся телом а
// и неподвижным телом b. Тело а движется на расстояние distance
// функция возвращает самую первую во ремени коллизию
bool PhysicEngine3D::checkCollision(Element &a, v3 direction, Element &b, Collision &collision)
{
	// каждый треугольник движущегося тела при движении образует призму
	// проверяем пересечение этой призмы со всеми треугольниками второго тела

	Plane at;
	if(a.triangle)
	for(vector<Triangle>::iterator it = a.triangle->data.begin(); it != a.triangle->data.end(); it++)
	{
		at.base = a.matrix * a.vertex->data[it->a[0]];
		at.a	= a.matrix * a.vertex->data[it->a[1]] - at.base;
		at.b	= a.matrix * a.vertex->data[it->a[2]] - at.base;

		if( ((at.a*at.b) & direction) > 0) // треугольник движется вперёд
		{
			for(vector<Triangle>::iterator jt = b.triangle->data.begin(); jt != b.triangle->data.end(); jt++)
			{
				Plane bt;
				bt.base = b.matrix * b.vertex->data[jt->a[0]];
				bt.a	= b.matrix * b.vertex->data[jt->a[1]] - bt.base;
				bt.b	= b.matrix * b.vertex->data[jt->a[2]] - bt.base;

				Collision newCollision;
				newCollision.time = INF;
				if(checkCollisionMTrgTrg(at, direction, bt, newCollision))
				{
					if(collision.time > newCollision.time)
						collision = newCollision;
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
	Element el;

	Interface::PositionKind pos = object->getPositionKind();
	ProcessKind::ProcessKind process = object->getProcessKind();
	if(process == ProcessKind::uni && pos == Interface::local)
	{
		object->changePositionKind(Interface::global);
		pos = object->getPositionKind();
	}

	el.object	= object;
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

