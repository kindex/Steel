/*id*********************************************************
	File: graph/opengl/opengl_private.h
	Unit: opengl
	Part of: Steel engine
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Приватные данные для OpenGL движка
 ************************************************************/

#include "opengl_private.h"

namespace opengl
{

const TexCoords* GraphShadow::getTexCoords(const MaterialStd::TextureStd& texture)
{
	if (texture.texCoordsUnit < texCoords.size())
	{
		return texCoords[texture.texCoordsUnit];
	}
	else
	{
		if (!texCoords.empty())
		{
			return texCoords[0];
		}
	}
	return NULL;
}

void GraphShadow::fill(GameObject* object)
{
	Shadow::fill(object);
}

LightShadow::LightShadow(): 
	light(NULL), 
	object(NULL)
{}

void GraphShadow::calculateAABB()
{
	aabb.clear();

	if (vertexes != NULL)
	{
		for EACH_CONST(Vertexes, *vertexes, i)
		{
			aabb.merge(*i);
		}
	}
	aabbCalculated = true;
}

bool GraphShadow::isCrossingLight(const LightShadow* light)
{
	AABB realaabb = aabb;
	realaabb.mul(realPosition);
	return realaabb.isCrossingSphere(light->position, light->light->maxDistance);
}

} // namespace opengl