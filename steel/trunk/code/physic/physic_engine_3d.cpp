
#include "physic_engine_3d.h"

using namespace std;


bool PhysicEngine3D::process(steel::time globalTime, steel::time time)
{
	element.clear();
	for(vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		prepare(*it);

	v3 acc = g*(float)time;
	for(vector<Element>::iterator it = element.begin(); it != element.end(); it++)
	{
		Element &el = *it;
		PhysicInterface &o = *el.obj;

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

		v3 p = o.getPosition();
		p += o.getVelocity()*(float)time;
		o.setPosition(p);

	}

	return true;
}


bool PhysicEngine3D::prepare(PhysicInterface *object, matrix44 matrix, PhysicInterface *parent)
{
	Element el;
	el.obj = object;
	el.parent = parent;

	element.push_back(el);
/*	MATRIX4X4 cur_matrix, new_matrix;

	cur_matrix = object->getMatrix();
	new_matrix = matrix*cur_matrix;
*/
	PhysicInterface &o = *(PhysicInterface*)object;
	PhysicInterfaceList children = o.getPChildrens();
	for(PhysicInterfaceList::iterator it=children.begin();	it != children.end(); it++)
		if(!prepare(*it)) return false;

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

