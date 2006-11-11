/*id*********************************************************
	File: objects/ps/ps_renderer.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Несколько вариантов рендерера для системы частиц
************************************************************/
#include "ps_renderer.h"
#include "../../res/res_main.h"

void SpriteRenderer::ProcessGraph(const GraphEngineInfo &info)
{
	initSprites(vertex.data.size()/4, set->particles.size());

	v3 pos_diff = particleSystem->getPosition().getTranslation();
	eye = info.cameraEye;

	int cnt = set->particles.size();
	for(int i=0; i<cnt; i++)
	{
		int i4 = i*4;
		v3 pos = set->particles[i]->position - pos_diff;

		v3 dir;
//В зависимости от типа выравнивания рассчитываем перпендикуляр к плоскости спрайта (dir).
//cameraDirection – ось камеры
//cameraEye – положение камеры
//pos – положение спрайта
		switch(align)
		{
			case SPRITE_ALIGN_SCREEN: dir = -info.cameraDirection; break;
			case SPRITE_ALIGN_CAMERA: dir = info.cameraEye - pos; break;
			case SPRITE_ALIGN_Z: dir = info.cameraEye - pos; dir.z = 0; break;
			case SPRITE_ALIGN_CUSTOM: dir = customAlign; break;
		}

		dir.normalize();
		// per1 и per2 - это направляющие сторон спрайта
		v3 per1(-dir.y, dir.x, 0); // перендикуляр к dir
		per1.normalize();
		v3 per2 = dir.vectorProduct(per1); // перпендикуляр к dir и per1
		per1 *= set->particles[i]->size; // умножаем на размер спарйта
		per2 *= set->particles[i]->size;

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

aabb SpriteRenderer::getFrame()
{
	aabb frame;
	for(steel::vector<v3>::iterator it = vertex.data.begin(); it != vertex.data.end(); it++)
		frame.merge(*it);

	return frame;
}

void SpriteRenderer::initSprites()
{
	face.resize(1);
	face[0].material = m;

	face[0].triangles = new Triangles;
	face[0].triangles->id = objectIdGenerator.genUid();
	face[0].triangles->changed = false;

	normal.id = objectIdGenerator.genUid();
	vertex.id = objectIdGenerator.genUid();
	vertex.changed = true;
	normal.changed = true;

	texCoords.changed = false;
	texCoords.id = objectIdGenerator.genUid();
}

void SpriteRenderer::initSprites(int begin, int end)
{
	if(begin == end) return ;

	vertex.data.resize(end*4);
	normal.data.resize(end*4);
	face[0].triangles->data.resize(end*2);

	for(int j = begin; j<end; j++)
	{
		for(int i=0; i<3; i++)
		{
			face[0].triangles->data[j*2 + 0].a[i] = j*4 + 3-i;
			face[0].triangles->data[j*2 + 1].a[i] = j*4 + 3-(i+2)%4;
		}
	}

	texCoords.data.resize(end*4);
	for(int i=begin; i<end; i++)
	{
		texCoords.data[i*4 + 0] = v2(0, 0);
		texCoords.data[i*4 + 1] = v2(1, 0);
		texCoords.data[i*4 + 2] = v2(1, 1);
		texCoords.data[i*4 + 3] = v2(0, 1);
	}
}

bool SpriteRenderer::initParticles()
{
	if(!(m = resMaterial.add(conf->gets("material")))) abort_init("res ps renderer", "Cannot find material " + conf->gets("material"));

	std::string salign = conf->gets("align", "screen"); // align;
	if(salign == "screen")	align = SPRITE_ALIGN_SCREEN; else
	if(salign == "z")		align = SPRITE_ALIGN_Z; else
	if(salign == "camera")	align = SPRITE_ALIGN_CAMERA;
	else 
	{
		customAlign = conf->getv3("align", v3(1,0,0));
		if(customAlign.getSquaredLengthd()<EPSILON2)	return false;
		align = SPRITE_ALIGN_CUSTOM;
	}

	initSprites();
	initSprites(0, set->particles.size());

	return true;
}

bool ObjectPSRenderer::initParticles()
{

	return true;
}

void ObjectPSRenderer::ProcessGraph(const GraphEngineInfo &info)
{
	int newSize = set->particles.size();
	int oldSize = children.size();
	if(newSize > oldSize)
	{
		children.resize(newSize);
		for(int i = oldSize; i < newSize; i++)
		{
			children[i] = new GameObjModel;
			ScriptLine line;

			line.set("solid			sphere/sphere2	0,0,0	0,0,0	0.2");
			children[i]->init(line);
			children[i]->setPositionKind(POSITION_GLOBAL);
		}
	}

	if(newSize < oldSize)
	{
		for(int i = newSize; i < oldSize; i++)
		{
			delete children[i];
		}
		children.resize(newSize);
	}

	for(int i = 0; i < newSize; i++)
	{
		ObjectPosition pos = children[i]->getPosition();
		pos.setTranslation(set->particles[i]->position);
		children[i]->setPosition(pos);
	}
}