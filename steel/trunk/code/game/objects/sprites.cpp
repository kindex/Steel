#include "sprites.h"
#include "../../common/utils.h"

using namespace std;

bool Sprites::init(ScriptLine &s, ResCollection &res)
{
	if(!GameObj::init(s, res)) return false;
	
	if(s.count()<3) return false;
	m = (Material*)res.add(Res::material, s.gets(3));
	if(!m) return false;

	sprite.resize(20);
	for(unsigned int i=0; i<sprite.size(); i++)
	{
		sprite[i].pos = v3(frand(), frand(), frand());
		sprite[i].size = 0.15f;
	}

	vertex.data.resize(sprite.size()*4);
	vertex.changed = true;
	vertex.id = res.genUid();

	normal.data.resize(sprite.size()*4);
	normal.changed = true;
	normal.id = res.genUid();

	face.resize(1);
	face[0].material = m;

	face[0].triangles.data.resize(sprite.size()*2);
	face[0].triangles.id = res.genUid();
	face[0].triangles.changed = false;

	for(unsigned int j=0; j<sprite.size(); j++)
	{
		for(int i=0; i<3; i++)
		{
			face[0].triangles.data[j*2 + 0].a[i] = j*4 + 3-i;
			face[0].triangles.data[j*2 + 1].a[i] = j*4 + 3-(i+2)%4;
		}
	}

	mapCoords.changed = false;
	mapCoords.id = res.genUid();
	mapCoords.data.resize(sprite.size()*4);
	for(unsigned int i=0; i<sprite.size(); i++)
	{
		mapCoords.data[i*4 + 0] = v2(0, 0);
		mapCoords.data[i*4 + 1] = v2(1, 0);
		mapCoords.data[i*4 + 2] = v2(1, 1);
		mapCoords.data[i*4 + 3] = v2(0, 1);
	}

	return true;
}

void Sprites::processGraph(v3	cameraEye)
{
	eye = cameraEye;

	int cnt = sprite.size();
	for(int i=0; i<cnt; i++)
	{
		v3 &pos = sprite[i].pos;
		v3 dir = eye - pos;
//		dir = v3(1,0,0);

		dir.normalize();
		v3 per1(-dir.y, dir.x, 0); // перендикул€р к dir
		per1.normalize();
		v3 per2 = dir.vectorProduct(per1);
		per1 *= sprite[i].size;
		per2 *= sprite[i].size;

		vertex.data[i*4 + 0]  = pos + per1 - per2;
		vertex.data[i*4 + 1]  = pos + -per1 - per2;
		vertex.data[i*4 + 2]  = pos + -per1 + per2;
		vertex.data[i*4 + 3]  = pos + per1 + per2;

		for(int j=0; j<4; j++)
			normal.data[i*4 + j] = dir;
	}
}

aabb Sprites::getFrame()
{
	aabb frame;
	for(vector<v3>::iterator it = vertex.data.begin(); it != vertex.data.end(); it++)
		frame.merge(*it);

	return frame;
}
