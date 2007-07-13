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
#include "../material.h"

namespace opengl
{

const TexCoords* GraphShadow::getTexCoords(const TextureStd& texture)
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
		for EACH_CONST(VertexVector, *vertexes, i)
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

void GraphShadow::calculateEgdes()
{
	if (vertexes != NULL && !vertexes->empty() )
	{
		int faceNumber = 0;
		edges.resize(faceMaterials->size());
		for EACH_CONST(FaceMaterialVector, *faceMaterials, faces)
		{
			if (!faces->faces->triangles.changed &&
				faces->faces->triangles.getId() == edges[faceNumber].trianglesId)
			{
				continue;
			}
			edges[faceNumber].trianglesId = faces->faces->triangles.getId();

			svector<pvector<int> > vertexesp;
			if (!(faces->faces->triangles.empty() && faces->faces->quads.empty()))
			{
				EdgeVector& edgesp = edges[faceNumber];
				edgesp.clear();
				vertexesp.clear();
				vertexesp.resize(vertexes->size());

				int i = 0;
				for EACH_CONST(TriangleVector, faces->faces->triangles, it)
				{
					for (int j = 0; j<3; j++)
					{
						int vert1 = it->a[j + 0];
						int vert2 = it->a[(j + 1)%3];

						bool found = false;
						int eind = -1;
						for EACH(pvector<int>, vertexesp[vert1], edgeIndex)
						{
							ShadowEdge* edge = &edgesp[*edgeIndex];
							if (edge->faceCount == 1 && 
								edge->vertex[0] == vert1 && edge->vertex[1] == vert2 ||
								edge->vertex[0] == vert2 && edge->vertex[1] == vert1)
							{
								// found second
								found = true;
								eind = *edgeIndex;
								break;
							}
						}

						if (found)
						{
							edgesp[eind].faceCount++;
							edgesp[eind].faces[1] = i;
							vertexesp[vert1].push_back(eind);
							vertexesp[vert2].push_back(eind);
						}
						else
						{
							vertexesp[vert1].push_back(edgesp.size());
							vertexesp[vert2].push_back(edgesp.size());
							edgesp.push_back(ShadowEdge(i, vert1, vert2));
						}
					}
					i++;
				}
			}
			faceNumber++;
		}
	}
}

bool LightShadow::needToBeRendered(const Flags& flags) const
{
	bool ok =	 light->castShadows && flags.current.shadowLighting || 
				!light->castShadows && flags.current.simpleLighting;

	if (flags.current.onlyLight != 0)
	{
		ok = ok && light->id == flags.current.onlyLight;
	}
	return ok;
}

} // namespace opengl