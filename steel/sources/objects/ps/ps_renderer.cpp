/*id*********************************************************
	File: objects/ps/ps_renderer.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Несколько вариантов рендерера для системы частиц
************************************************************/
#include "ps_renderer.h"
#include "../../res/res_main.h"
#include "../../math/sprite.h"
#include "../game_object_factory.h"
#include "../combiner/combiner.h"


void SpriteRenderer::updateSpritePositions(IN const ProcessInfo& info)
{
	initSprites(vertexes.size()/4, set->particles.size());

	cameraPosition = info.camera.getPosition();

	int cnt = set->particles.size();
	for(int i=0; i < cnt; i++)
	{
		int i4 = i*4;
        v3 dx;
        v3 dy;
		calculateSprite(info.camera,
						set->particles[i]->position,
                        POSITION_GLOBAL,
						v2(set->particles[i]->size, set->particles[i]->size),
						align,
						customAlign,
						vertexes[i4 + 0],
						vertexes[i4 + 1],
						vertexes[i4 + 2],
						vertexes[i4 + 3],
						normals[i4 + 0],
                        dx,
                        dy);

		normals[i4 + 1] = normals[i4 + 2] = normals[i4 + 3] = normals[i4 + 0];
	}
}

AABB SpriteRenderer::getFrame()
{
	AABB frame;
	for EACH(pvector<v3>, vertexes, it)
	{
		frame.merge(*it);
	}

	return frame;
}

void SpriteRenderer::initSprites()
{
	face.resize(1);
	face[0].material = material;

	face[0].faces = new Faces;
	face[0].faces->quads.id = objectIdGenerator.genUid();
	face[0].faces->quads.changed = false;

	normals.id = objectIdGenerator.genUid();
	vertexes.id = objectIdGenerator.genUid();
	vertexes.changed = true;
	normals.changed = true;

	texCoords.changed = false;
	texCoords.id = objectIdGenerator.genUid();
}

void SpriteRenderer::initSprites(int begin, int end)
{
	if (begin == end)
    {
        return;
    }

	vertexes.resize(end*4);
	normals.resize(end*4);
	face[0].faces->quads.resize(end);

	for(int j = begin; j < end; j++)
	{
		for(int i=0; i < 4; i++)
		{
			face[0].faces->quads[j].a[i] = j*4 + i;
		}
	}

	texCoords.resize(end*4);
	for(int i=begin; i<end; i++)
	{
		texCoords[i*4 + 0] = v2(0, 0);
		texCoords[i*4 + 1] = v2(0, 1);
		texCoords[i*4 + 2] = v2(1, 1);
		texCoords[i*4 + 3] = v2(1, 0);
	}
}

bool SpriteRenderer::InitFromConfig(IN Config& _conf)
{
	conf = &_conf;
	Config* materailConfig = conf->find("material");
	if (materailConfig == NULL)
	{
		return false;
	}
	material = createMaterial(materailConfig);

	std::string salign = conf->gets("align", "screen"); // align;
	if (salign == "screen")
    {
        align = SPRITE_ALIGN_SCREEN; 
    }
    else if (salign == "z")
    {
        align = SPRITE_ALIGN_Z;
    }
    else if (salign == "camera")
    {
        align = SPRITE_ALIGN_CAMERA;
    }
	else 
	{
		customAlign = conf->getv3("align", v3(1,0,0));
		if (customAlign.getSquaredLengthd() < EPSILON2)
        {
            return false;
        }
		align = SPRITE_ALIGN_CUSTOM;
	}

	return true;
}

bool SpriteRenderer::initParticles()
{
	initSprites();
	initSprites(0, set->particles.size());
	return true;
}

bool SpriteRenderer::updateInformation(Engine& engine, IN const InterfaceId id)
{
	if (id == INTERFACE_GRAPH)
	{
		GraphInterface &gengine = *dynamic_cast<GraphInterface*>(&engine);

		updateSpritePositions(gengine.getProcessInfo());

		gengine.setVertexes(&vertexes);
		gengine.setNormals(&normals);
		gengine.setFaceMaterials(&face);
		gengine.setTexCoordsCount(1);
		gengine.setTexCoords(0, &texCoords);
		return true;
	}
	return false;
}

bool ObjectPSRenderer::initParticles()
{

	return true;
}

bool ObjectPSRenderer::InitFromConfig(Config& _conf)
{
    conf = &_conf;
    Config* c = conf->find("object");
    if (c == NULL)
    {
        return false;
    }
    particle_config = dynamic_cast<ConfigStruct*>(c);

    return particle_config != NULL;
}

bool ObjectPSRenderer::updateInformation(IN OUT Engine& engine, IN const InterfaceId interfaceId)
{
    if (interfaceId && INTERFACE_GRAPH == INTERFACE_GRAPH)
    {
        GraphInterface* gengine = dynamic_cast<GraphInterface*>(&engine);
        size_t index = 0;
        for EACH(std::vector<Object>, objects, it)
        {
            if (!it->inserted)
            {
                it->inserted = true;
                gengine->addChild(it->object);
            }
            gengine->setCurrentObject(it->object, interfaceId);
            gengine->setPositionKind(POSITION_GLOBAL);
            ObjectPosition pos = ObjectPosition::getIdentity();
            pos.setTranslation(set->particles[index]->position);
            gengine->setPosition(pos);

            index++;
        }
        return true;
    }

    return false;
}

void ObjectPSRenderer::onParticleBorn(int index)
{
    particle_config->setValue("origin", createV3config(set->particles[index]->position));
    GraphObject* new_object = loadGraphObject(*particle_config, "");
    if (new_object == NULL)
    {
        return;
    }
    objects.push_back(Object(false, new_object));
}

/*void ObjectPSRenderer::ProcessGraph(const GraphEngineInfo &info)
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
*/
