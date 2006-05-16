
#include "physic_engine_3d.h"

#include "../math/plane.h"
#include "../common/utils.h"

using namespace std;

#define CONTACT_EPSILON (0.01f)

// Сердце физического движка
// обрабатывает движение одного объекта в указанный промежуток времени

bool PhysicEngine3D::processTime(PhysicInterface &o, steel::time globalTime, steel::time time, float &processedTime)
{
	ProcessKind::ProcessKind kind = o.getProcessKind();
	if(kind != ProcessKind::none) 
	{
		if(kind == ProcessKind::custom)
		{
			o.process(globalTime, time, this);
		}
		if(kind == ProcessKind::uni)
		{
			velocity v = o.getVelocity();
			v.translation += g*(float)time;	// gravitation
			o.setVelocity(v);
		}

		const matrix44 global = o.getGlobalMatrix();

		velocity v = o.getVelocity();

	//		velocity = v3(0,0, -10);

		v3 path = v.translation*(float)time;

		v3 oldPos = o.getPosition();
		v3 newPos = oldPos + path;

		Collision collision;
		collision.time = INF; // no collision

		Interface::PositionKind pos = o.getPositionKind();
		v3 dir; // global

		if(pos == Interface::local)
			dir = global*newPos-global*oldPos;
		else
			dir = newPos - oldPos;

		collisionDetection(o, dir, collision, &o);

		if(collision.time<=1 + EPSILON)
		{
			float len = path.getLength();
			len *= collision.time;
			len -= CONTACT_EPSILON;
			if(len>EPSILON)
			{
				path = path.getNormalized()*len;
				newPos = oldPos + path;
				o.setPosition(newPos);
			}

			collision.a = &o;

			if(findCollision(collision) == 0)
			{
				if(helper) // draw collision
				{
					helper->drawVector(Line(
						collision.point,
						collision.normal.getNormalized()*0.1f
						), 0.0f,0.0f, v4(1.0f,0.0f,0.0f,1.0f));
				}

				collisionReaction(collision);
				incCollision(collision);

				total.collisionCount++;
				processedTime = collision.time;
				return false;
			}
			else
				return true; // движение закончено - объект столкнулся с дургим обхектом повторно
		}
		else
		{
			o.setPosition(newPos);
		}
	}
	return true;
}

// обрабатывает движение одного объекта и его потомков
// если была коллизия, то двигает тело еще раз после коллизии (5 раз)
bool PhysicEngine3D::process(PhysicInterface &o, steel::time globalTime, steel::time time)
{
	float endProcessedTime = 0;
	float totalProcessedTime = 0;
	int collisionCount = 0;
	bool ended;
	do
	{
		ended = processTime(o, globalTime + totalProcessedTime*time, time*(1-totalProcessedTime), endProcessedTime);
		totalProcessedTime += time*(1-totalProcessedTime)*endProcessedTime;
		collisionCount++;
	}
	while(!ended && collisionCount<5);

	PhysicInterfaceList c = o.getPChildrens();

	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		process(**it, globalTime, time);

	if(helper && o.getProcessKind() != ProcessKind::none) // draw velocity
	{
		helper->drawVector(Line(
			o.getGlobalMatrix()*v3(),
			o.getGlobalVelocity().translation.getNormalized()
			), 0,0, v4(0,1,0,1));
	}
	if(helper) // draw AABB
	{
		aabb frame = o.getPFrame();
		if(!frame.empty())
		{
			frame.mul(o.getGlobalMatrix());
			v4 color;
			
			helper->drawBox(frame, 0,0, color);
		}
	}

	return true;
}

// обрабатывает движение всех объектов
bool PhysicEngine3D::process(steel::time globalTime, steel::time time)
{
	if(helper) // draw velocity
		helper->setTime(globalTime);

	allCollisions.clear();

/*	element.clear();*/
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		prepare(*it);

	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
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

int PhysicEngine3D::findCollision(const Collision collision)
{
	std::map<Collision, int>::iterator it = allCollisions.find(collision);
	if(it == allCollisions.end()) 
		return false;
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

bool PhysicEngine3D::collisionReactionUniNone(const Collision collision)
{
	v3 t = collision.normal.getNormalized();

	velocity V1 = collision.a->getVelocity(); v3 v1 = V1.translation;
	velocity V2 = collision.b->getVelocity(); v3 v2 = V2.translation;

	float v1t = t&v1; float v2t = t&v2;
	v3 v1sr = v1 - t*v1t;

	float u1t = v2t - v1t;

	v3 u1 = v1sr + t*u1t;

	V1.translation = u1;
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

//	if(v1t*v2t>0) return false;

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

	if (D<EPSILON) D = 0;//	assert(D>=0, "collisionReaction D>=0");

	float u2ta = (-B + sqrt(D))/(2*A);	float u1ta = (It - m2*u2ta)/m1; 
	float u2tb = (-B - sqrt(D))/(2*A);	float u1tb = (It - m2*u2tb)/m1;

	float u1t, u2t;

	if(sgn(v1t - v2t) != sgn(u1ta - u2ta))
		u1t = u1ta,	u2t = u2ta;
	else
		u1t = u1tb,	u2t = u2tb;
	
	v3 v1sr = v1 - t*v1t;
	v3 v2sr = v2 - t*v2t;
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
		if(checkCollision(a, distance, b, newCollision))
		{
			if(collision.time > newCollision.time)
			{
				collision = newCollision;
				collision.b = &b;
			}
		}
	}

	PhysicInterfaceList c = b.getPChildrens();
	for(PhysicInterfaceList::iterator it = c.begin(); it != c.end(); it++)
		collisionDetection(a, **it, distance, collision, clip);
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
		
		if(collision.time > time+ 1.0e-4)
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
// 3. ребро - ребро
bool PhysicEngine3D::checkCollisionMTrgTrg(Plane a, v3 direction, Plane b, Collision &collision)
{
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

	return collision.time<=1;
}


// проверяет коллизию между движушимся телом а
// и неподвижным телом b. Тело а движется на расстояние distance
// функция возвращает самую первую во ремени коллизию
bool PhysicEngine3D::checkCollision(PhysicInterface &a, v3 distance, PhysicInterface &b, Collision &collision)
{
	// каждый треугольник движущегося тела при движении образует призму
	// проверяем пересечение этой призмы со всеми треугольниками второго тела

	aabb newframe1 = a.getPFrame();
	newframe1.mul(a.getGlobalMatrix());
	newframe1.add(distance);

	aabb newframe2 = b.getPFrame();
	newframe2.mul(b.getGlobalMatrix());

	newframe1.cross(newframe2);

/*	newframe1.max += v3(CONTACT_EPSILON, CONTACT_EPSILON, CONTACT_EPSILON);
	newframe1.min -= v3(CONTACT_EPSILON, CONTACT_EPSILON, CONTACT_EPSILON);
*/
	if(newframe1.empty()) return false;


	Triangles *t = a.getTriangles();
	Vertexes *v = a.getPVertexes();
	matrix44 matrix = a.getGlobalMatrix();

	Triangles	*t2	= b.getTriangles();
	Vertexes	*v2	= b.getPVertexes();
	matrix44 matrix2 = b.getGlobalMatrix();


	vector<Plane> bp;

	for(vector<Triangle>::iterator jt = t2->data.begin(); jt != t2->data.end(); jt++)
	{
		Plane bt;
		bt.base = matrix2 * v2->data[jt->a[0]];
		bt.a	= matrix2 * v2->data[jt->a[1]] - bt.base;
		bt.b	= matrix2 * v2->data[jt->a[2]] - bt.base;
		aabb aabb2;
		aabb2.merge(bt.base);
		aabb2.merge(bt.base + bt.a);
		aabb2.merge(bt.base + bt.b);

		if(newframe1.intersect(aabb2))
		{
			bp.push_back(bt);
		}
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

					Collision newCollision;
					newCollision.time = INF;
					if(checkCollisionMTrgTrg(at, distance, bt, newCollision))
					{
						if(collision.time > newCollision.time)
							collision = newCollision;
						ok = true;
					}
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

