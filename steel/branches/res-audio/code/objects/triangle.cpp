#include "triangle.h"
#include "../res/res_main.h"

bool GameTriangleObj::init(ScriptLine	&s)
{
	if(!GameObj::init(s)) return false;

	vertex.changed = false;
	vertex.setId(objectIdGenerator.genUid());
	vertex.data.resize(3);
	vertex.data[0].set(0,0,0);
	vertex.data[1].set(1,0,0);
	vertex.data[2].set(0,1,0);

	frame.clear();
	frame.merge(vertex.data[0]);
	frame.merge(vertex.data[1]);
	frame.merge(vertex.data[2]);


	texCoords.changed = false;
	texCoords.setId(objectIdGenerator.genUid());
	texCoords.data.resize(3);
	texCoords.data[0].set(0,0);
	texCoords.data[1].set(1,0);
	texCoords.data[2].set(0,1);

	triangle.changed = false;
	triangle.setId(objectIdGenerator.genUid());
	triangle.data.resize(2);
	triangle.data[0].a[0] = 0;
	triangle.data[0].a[1] = 1;
	triangle.data[0].a[2] = 2;

	if(triangle.data.size()>=2)
	{
		triangle.data[1].a[0] = 0;
		triangle.data[1].a[1] = 2;
		triangle.data[1].a[2] = 1;
	}

	face.resize(1);

	face[0].name = s.gets(3);
	face[0].material = resMaterial.add(face[0].name);
	face[0].triangles = &triangle;

	return true;
}


