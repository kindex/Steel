#include "graph_helper.h"

using namespace std;

void GraphHelper::setTime(const steel::time _time) 
{
	time = _time; 
	for(vector<LineElement>::iterator it = lines.begin(); it != lines.end(); )
		if(it->end < time)	lines.erase(it);
		else it++;

	vectors.clear();
/*	for(vector<LineElement>::iterator it = vectors.begin(); it != vectors.end(); )
		if(it->end < time)	vectors.erase(it);
		else it++;*/
}

void GraphHelper::drawVector(const Line line, const steel::time duration, const steel::time fade, const v4 color)
{
	vectors.push_back(LineElement(line, time, time + fade, time + duration, color));
}

void GraphHelper::processGraph(v3	cameraEye, v3 cameraDirection)
{
	if(face.empty())
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

	vertex.changed = true;
	vertex.id = 0;

	face[0].triangles.changed = true;
	face[0].triangles.id = 0;

	vertex.data.resize(vectors.size()*3);
	face[0].triangles.data.resize(vectors.size());

	int i = 0;
	for(vector<LineElement>::iterator it = vectors.begin(); it != vectors.end(); it++)
	{
		v3 b = it->line.a * cameraDirection.getNormalized()*0.01f;
		vertex.data[i*3 + 0].set(it->line.base - b);
		vertex.data[i*3 + 1].set(it->line.base + it->line.a);
		vertex.data[i*3 + 2].set(it->line.base + b);

		face[0].triangles.data[i].a[0] = i*3 + 0;
		face[0].triangles.data[i].a[1] = i*3 + 1;
		face[0].triangles.data[i].a[2] = i*3 + 2;
		i++;
	}

}

