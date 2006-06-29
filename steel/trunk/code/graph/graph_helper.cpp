#include "graph_helper.h"

void GraphHelper::setTime(const steel::time _time) 
{
	time = _time; 

	lines.clear();

	vectors.clear();
	boxes.clear();

/*	for(vector<LineElement>::iterator it = vectors.begin(); it != vectors.end(); )
		if(it->end < time)	vectors.erase(it);
		else it++;*/
}

void GraphHelper::drawVector(const Line line, const steel::time duration, const steel::time fade, const color4f color)
{
	vectors.push_back(LineElement(line, time, time + fade, time + duration, color));
}

void GraphHelper::drawLine(const Line line, const steel::time duration, const steel::time fade, const color4f color)
{
	lines.push_back(LineElement(line, time, time + fade, time + duration, color));
}

void GraphHelper::drawBox(const aabb box, const steel::time duration, const steel::time fade, const color4f color)
{
	boxes.push_back(BoxElement(box, time, time + fade, time + duration, color));
}

void GraphHelper::processGraph(v3	cameraEye, v3 cameraDirection)
{
/*	if(face.empty())
	{
		face.resize(1);
		face[0].material = new Material;
		face[0].material->map.resize(1);
		face[0].material->blend = false;
		face[0].material->id = 0;

		face[0].material->map[0].color.set(0.0f, 1.0f, 0.0f, 0.5f);
		face[0].material->map[0].kind = MapKind::color;
		face[0].material->map[0].mode = MapMode::replace;
	}
*/
	vertex.changed = true;
	vertex.id = 0;

	/*face[0].triangles.changed = true;
	face[0].triangles.id = 0;
*/

	vertex.data.resize(vectors.size()*4 + lines.size()*2 + boxes.size()*8);
//	face[0].triangles.data.resize(vectors.size());
	glines.resize(vectors.size()*3 + lines.size() + boxes.size()*12);

	int vi = 0;
	int li = 0;
	for(steel::vector<LineElement>::iterator it = vectors.begin(); it != vectors.end(); it++)
	{
		v3 b = it->line.a * cameraDirection.getNormalized()*0.1f;
		vertex.data[vi + 0].set(it->line.base);
		vertex.data[vi + 1].set(it->line.base + it->line.a);

		vertex.data[vi + 2].set(it->line.base + it->line.a*0.8f + b);
		vertex.data[vi + 3].set(it->line.base + it->line.a*0.8f - b);

		glines[li + 0].a[0] = vi + 0;
		glines[li + 0].a[1] = vi + 1;
		glines[li + 1].a[0] = vi + 1;
		glines[li + 1].a[1] = vi + 2;
		glines[li + 2].a[0] = vi + 1;
		glines[li + 2].a[1] = vi + 3;

		vi += 4;
		li += 3;
	}

	for(vector<LineElement>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		vertex.data[vi + 0].set(it->line.base);
		vertex.data[vi + 1].set(it->line.base + it->line.a);

		glines[li + 0].a[0] = vi + 0;
		glines[li + 0].a[1] = vi + 1;

		vi += 2;
		li += 1;
	}

	for(vector<BoxElement>::iterator it = boxes.begin(); it != boxes.end(); it++)
	{
		aabb &b = it->box;

		steel::svector<v3> r;
		b.getVertexes(r);
		for(int i=0;  i<8; i++)
			vertex.data[vi + i].set(r[i]);

#define GSET(n, a, b)	glines[li + n].set(vi + a, vi + b);

		GSET(0, 0, 1);
		GSET(1, 2, 3);
		GSET(2, 4, 5);
		GSET(3, 6, 7);

		GSET(4, 0, 2);
		GSET(5, 2, 6);
		GSET(6, 6, 4);
		GSET(7, 4, 0);

		GSET(8, 1, 3);
		GSET(9, 3, 7);
		GSET(10, 7, 5);
		GSET(11, 5, 1);
		
		vi += 8;
		li += 12;
	}


}

