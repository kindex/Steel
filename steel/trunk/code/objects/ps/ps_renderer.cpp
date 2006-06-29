#include "ps_renderer.h"
#include "../../res/res_main.h"

void SpriteRenderer::processGraph(v3	cameraEye, v3 cameraDirection)
{
	initSprites(vertex.data.size()/4, set->particles.size());

	v3 pos_diff = particleSystem->getPosition().getTranslation();
	eye = cameraEye;

	int cnt = set->particles.size();
	for(int i=0; i<cnt; i++)
	{
		int i4 = i*4;
		v3 pos = set->particles[i]->position - pos_diff;

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
	if(salign == "screen")	align = SpriteAlign::screen; else
	if(salign == "z")		align = SpriteAlign::z; else
	if(salign == "camera")	align = SpriteAlign::camera;
	else 
	{
		customAlign = conf->getv3("align", v3(1,0,0));
		if(customAlign.getSquaredLengthd()<EPSILON2)	return false;
		align = SpriteAlign::custom;
	}

	initSprites();
	initSprites(0, set->particles.size());

	return true;
}

bool ObjectPSRenderer::initParticles()
{

	return true;
}

void ObjectPSRenderer::processGraph(v3 cameraEye, v3 cameraDirection)
{
	int newSize = set->particles.size();
	int oldSize = children.size();
	if(newSize > oldSize)
	{
		children.resize(newSize);
		childrenModel.resize(newSize);
		for(int i = oldSize; i < newSize; i++)
		{
			childrenModel[i] = new GameObjModel;
			children[i] = childrenModel[i];
			ScriptLine line;

			line.set("solid			sphere/sphere2	0,0,0	0,0,0	0.2");
			childrenModel[i]->init(line);
			childrenModel[i]->setPositionKind(PositionKind::global);
		}
	}

	for(int i = 0; i < newSize; i++)
	{
		ObjectPosition pos = childrenModel[i]->getPosition();
		pos.setTranslation(set->particles[i]->position);
		childrenModel[i]->setPosition(pos);
	}

}
