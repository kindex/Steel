
#include "physic_engine_3d.h"

using namespace std;


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

		Element *second = NULL;

		if(collisionDetection(el, global*newPos-global*oldPos, second))
		{
			second->collisionCount++;
			el.collisionCount++;

			v3 secondVelocity = second->object->getVelocity();
			v3 firstVelocity = o.getVelocity();
			second->object->setVelocity(-secondVelocity);
			o.setVelocity(-firstVelocity);
		}
		else
		{
			o.setPosition(newPos);
//			o.setPosition(p);
		}
		el.frame.min += path;
		el.frame.max += path;

	}

	return true;
}

struct MTriangle
{
// a - точка остчЄта (нижний угол)
// b,c - напр€вл€ющие двух сторон
// z - направление движени€ ("высота" призмы)
	v3	a,b,c,z;
};

struct CTriangle
{
// a - точка остчЄта (нижний угол)
// b,c - напр€вл€ющие двух сторон
	v3 a,b,c;
};

/*bool checkCollisionMTrgTrg(MTriangle a, CTriangle b, Collision &collision)
{
	return false;
}
*/

// провер€ет коллизию между движушимс€ телом а
// и неподвижным телом b. “ело а движетс€ на рассто€ние distance
// функци€ возвращает самую первую во ремени коллизию
bool PhysicEngine3D::checkCollision(Element &a, v3 distance, Element &b, Collision &collision)
{
	// каждый треугольник движущегос€ тела при движении образует призму
	// провер€ем пересечение этой призмы со всеми треугольниками второго тела

	MTriangle at;
	at.z = distance;

	for(vector<Triangle>::iterator it = a.triangle->data.begin(); it != a.triangle->data.end(); it++)
	{
		at.a = a.vertex->data[it->a[0]];
		at.b = a.vertex->data[it->a[1]] - at.a;
		at.c = a.vertex->data[it->a[2]] - at.a;

		if(at.b.vectorProduct(at.c).dotProduct(distance)>0) // треугольник движетс€ вперЄд
		{
			for(vector<Triangle>::iterator jt = b.triangle->data.begin(); jt != b.triangle->data.end(); jt++)
			{
				const v3 b1 = a.vertex->data[jt->a[0]];
				const v3 b2 = a.vertex->data[jt->a[1]];
				const v3 b3 = a.vertex->data[jt->a[2]];

				

			}

		}

	//a.vertex
	}

	return false;
}

// Check for collision
bool PhysicEngine3D::collisionDetection(Element &a, v3 distance, PElement &second)
{
	aabb newframe1 = a.frame;
	newframe1.add(distance);

	for(vector<Element>::iterator it = element.begin(); it != element.end(); it++)
		if(it->triangle && it->vertex && it->object != a.object)
		{
			Element &b = *it;
			
			if(newframe1.intersect(b.frame)) 
			{
				second = &b;
				return true;
			}
//			a.matrix
//			b.matrix
		}
		return false;
}

bool PhysicEngine3D::prepare(PhysicInterface *object, matrix44 parent_matrix, PhysicInterface *parent)
{
	if(!object) return false;
	Element el;
	el.object	= object;
	el.parent	= parent;
	el.vertex	= object->getPVertexes();
	el.triangle = object->getTriangles();
	el.frame	= object->getPFrame(); // local
	el.matrix   = object->getMatrix();
	el.collisionCount = 0;

	Interface::PositionKind pos = object->getPositionKind();
	if(pos == Interface::local)
		el.matrix = parent_matrix*el.matrix;

	el.frame.mul(el.matrix);

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

