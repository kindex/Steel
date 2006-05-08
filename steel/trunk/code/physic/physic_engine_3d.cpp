
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
			collision.b->collisionCount++;
			el.collisionCount++;

			v3 secondVelocity = collision.b->object->getVelocity();
			v3 firstVelocity = o.getVelocity();

			collision.b->object->setVelocity(-secondVelocity);
			o.setVelocity(-firstVelocity);

			double len = path.getLengthd();
			len *= collision.time;
			len -= CONTACT_EPSILON;
			if(len>0)
			{
				path = path.getNormalized()*len;
				newPos = oldPos + path;
				o.setPosition(newPos);
				el.frame.min += path;
				el.frame.max += path;
			}

			total.collisionCount++;
		}
		else
		{
			o.setPosition(newPos);
			el.frame.min += path;
			el.frame.max += path;
//			o.setPosition(p);
		}
	}
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
	if(isCrossTrgLine(b, Line(point, direction), k))
		if(k>=-EPSILON && k<=1+EPSILON)
			if(collision.time > k)
			{
				collision.time = k;
				collision.point = point;
				collision.normal = b.a*b.b; //нормаль к треугольнику
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
//			collision.normal = a.a*a.b; // TODO
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
	checkCollisionMVertexTrg(a.base, direction, b, collision);
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

