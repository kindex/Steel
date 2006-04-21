
#include "physic_engine_3d.h"

using namespace std;


bool PhysicEngine3D::process(steel::time speed)
{
	v3 acc = g*(float)speed;
	for(std::vector<PhysicElement>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		PhysicElement &el = *it;
		PhysicInterface &o = *el.obj;
		o.process(speed);

		v3 pos = el.matrix * v3(); // absolute
		v3 local_pos = o.getPMatrix() * v3(); // local (в системе координат родителя)

		v3 v = o.getVelocity();
		if(o.isMovable()) v += acc;// gravitation
		o.setVelocity(v);

		v3 targetPoint;
		string targetObj;
		coord moveSpeed;
		// если объект хочет двигаться к другому объекту
		if(o.getTarget(targetPoint, moveSpeed))
		{ // если он указал имя объекта
/*			if(!targetObj.empty())
			{
				if(tag.find(targetObj) == tag.end())
				{
					alog.msg("error physic", string("Cannot find object with id '") + targetObj + "'");
					return false;
				}
				int idx = tag[targetObj];
				matrix4 matrix =  objects[idx].matrix;
				v3 one;
				targetPoint = el.parentMatrix.GetInverse()*matrix*one;
			}*/
			// если не указал имя, то берём координаты (локальные)
			v3 dir = (targetPoint - local_pos).getNormalized(); // направление движения
			
//			o.setVelocity(dir*moveSpeed);
			coord len = (float)(moveSpeed*speed); // пройденное расстояние

			if(len>(targetPoint - local_pos).getLength())
			{
				local_pos = targetPoint;
				o.setTargetReached();
			}
			else
				local_pos += dir * len;
			
//			matrix4 inv = el.parentMatrix.GetInverse();

//			v3 local_pos =  inv* pos;

			matrix44 localM = o.getPMatrix();
			localM.a[12] = local_pos.x;
			localM.a[13] = local_pos.y;
			localM.a[14] = local_pos.z;
			o.setPMatrix(localM);
		}

/*		matrix4 m = (*it)->getMatrix();
		p += v*speed;
		(*it)->setPosition(p);*/
	}

	return true;
}

bool PhysicEngine3D::inject(PhysicInterface *object, matrix44 matrix, PhysicInterface *parent)
{
	PhysicElement el;
	el.obj = object;
	el.parent = parent;

/*	string name = object->getName();
	if(!name.empty())
	{
		if(tag.find(name) != tag.end())
		{
			alog.msg("error physic", string("Duplicated object id '") + name + "'");
			return false;
		}
		tag[name] = objects.size();
	}*/

	matrix44 cur_matrix, new_matrix;

	cur_matrix = object->getPMatrix();
	new_matrix = matrix*cur_matrix;

	el.matrix = new_matrix;
	el.parentMatrix = matrix;

	objects.push_back(el);


	PhysicInterface &o = *(PhysicInterface*)object;
	PhysicInterfaceList children = o.getPChildrens();
	for(PhysicInterfaceList::iterator it=children.begin();	it != children.end(); it++)
		if(!inject(*it, new_matrix)) return false;

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

