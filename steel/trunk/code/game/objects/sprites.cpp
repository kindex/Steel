/*id*********************************************************
    Unit: game/objects/sprites
    Part of: Steel engine
    Version: 1.0
    Authors:
        * def [Alexander Papyshev]
    Description:
		“ут содержатс€ классы дл€ рендеринга системы частиц
************************************************************/

#include "sprites.h"
#include "../../common/utils.h"

using namespace std;

bool Sprite::init(ScriptLine &s, ResCollection &_res)
{
	if(!GameObj::init(s, _res)) return false;

	res = &_res;
	
	if(s.count()<5) return false;
	m = (Material*)res->add(Res::material, s.gets(3));
	if(!m) return false;

	string salign = s.gets(7, "screen"); // align;
	if(salign == "screen")	align = SpriteAlign::screen; else
	if(salign == "z")		align = SpriteAlign::z; else
	if(salign == "camera")	align = SpriteAlign::camera;
	else 
	{
		customAlign = s.getv3(7);
		if(customAlign.getSquaredLengthd()<EPSILON2)	return false;
		align = SpriteAlign::custom;
	}

	sprite.resize(1);
	for(unsigned int i=0; i<sprite.size(); i++)
	{
		sprite[i].pos = v3(0, 0, 0);
		sprite[i].size = s.getf(8, 1);
	}
	initSprites();
	return true;
}

void Sprite::setAlign(const v3 _align)
{
	customAlign = _align;
}

void SpriteSystem::initSprites()
{
	vertex.data.resize(sprite.size()*4);
	vertex.changed = true;
	vertex.id = res->genUid();

	normal.data.resize(sprite.size()*4);
	normal.changed = true;
	normal.id = res->genUid();

	face.resize(1);
	face[0].material = m;

	face[0].triangles = new Triangles;
	face[0].triangles->data.resize(sprite.size()*2);
	face[0].triangles->id = res->genUid();
	face[0].triangles->changed = false;

	for(unsigned int j=0; j<sprite.size(); j++)
	{
		for(int i=0; i<3; i++)
		{
			face[0].triangles->data[j*2 + 0].a[i] = j*4 + 3-i;
			face[0].triangles->data[j*2 + 1].a[i] = j*4 + 3-(i+2)%4;
		}
	}

	texCoords.changed = false;
	texCoords.id = res->genUid();
	texCoords.data.resize(sprite.size()*4);
	for(unsigned int i=0; i<sprite.size(); i++)
	{
		texCoords.data[i*4 + 0] = v2(0, 0);
		texCoords.data[i*4 + 1] = v2(1, 0);
		texCoords.data[i*4 + 2] = v2(1, 1);
		texCoords.data[i*4 + 3] = v2(0, 1);
	}
}

void SpriteSystem::processGraph(v3	cameraEye, v3 cameraDirection)
{
	eye = cameraEye;

	int cnt = sprite.size();
	for(int i=0; i<cnt; i++)
	{
		int i4 = i*4;
		v3 &pos = sprite[i].pos;

		v3 dir;
//¬ зависимости от типа выравнивани€ рассчитываем перпендикул€р к плоскости спрайта (dir).
//cameraDirection Ц ось камеры
//cameraEye Ц положение камеры
//pos Ц положение спрайта
		switch(align)
		{
			case SpriteAlign::screen: dir = -cameraDirection; break;
			case SpriteAlign::camera: dir = cameraEye - pos; break;
			case SpriteAlign::z: dir = cameraEye - pos; dir.z = 0; break;
			case SpriteAlign::custom: dir = customAlign; break;
		}


		dir.normalize();
		// per1 и per2 - это направл€ющие сторон спрайта
		v3 per1(-dir.y, dir.x, 0); // перендикул€р к dir
		per1.normalize();
		v3 per2 = dir.vectorProduct(per1); // перпендикул€р к dir и per1
		per1 *= sprite[i].size; // умножаем на размер спарйта
		per2 *= sprite[i].size;

		// углы спарйта
		vertex.data[i4 + 0]  = pos + per1 - per2;
		vertex.data[i4 + 1]  = pos + -per1 - per2;
		vertex.data[i4 + 2]  = pos + -per1 + per2;
		vertex.data[i4 + 3]  = pos + per1 + per2;

		// нормали к углам спарйта
		normal.data[i4 + 0] = dir;
		normal.data[i4 + 1] = dir;
		normal.data[i4 + 2] = dir;
		normal.data[i4 + 3] = dir;
	}
}

aabb SpriteSystem::getFrame()
{
	aabb frame;
	for(vector<v3>::iterator it = vertex.data.begin(); it != vertex.data.end(); it++)
		frame.merge(*it);

	return frame;
}
