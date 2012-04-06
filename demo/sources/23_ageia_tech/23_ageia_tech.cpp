/*id*********************************************************
	Unit: 23 Ageia tech
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		PhysX test
 ************************************************************/
#include "23_ageia_tech.h"
#include "../19_ps/particle_calculator.h"
#include "../physx/physx.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

#include <NxPhysics.h>
#include <NxCooking.h>
#include <Stream.h>

class GraphObjectVisitor : public Visitor
{
public:
    GraphObjectVisitor(GameAgeiatech& game) : game(game) {}

private:
    bool visit(IN OUT GameObject* object)
    {
        GraphObject* g = dynamic_cast<GraphObject*>(object);

        if (g != NULL)
        {
            game.createSurface(*g);
        }
        return true;
    }

    GameAgeiatech& game;
};

GameAgeiatech::GameAgeiatech() :
	scene(NULL)
{}

NxActor* GameAgeiatech::createSurface(const GraphObject& object)
{
    NxVec3* fsVerts = NULL;
    NxU32* fsFaces = NULL;
//    NxVec3* fsNormals = NULL;

    // Initialize flat surface verts
    const VertexVector& vertexes = *object.getVertexes();
    size_t nbVerts = vertexes.size();
    fsVerts = new NxVec3[vertexes.size()];

    // Build flat surface
    for (size_t i = 0; i < vertexes.size(); i++)
    {
        fsVerts[i] = NxVec3(vertexes[i].x, vertexes[i].y+0.5f, vertexes[i].z); 
    }

    const TriangleVector& triangles = object.getAllFaces()->triangles;

    // Initialize flat surface faces
    size_t nbFaces = triangles.size();
    fsFaces = new NxU32[nbFaces*3];

    size_t k = 0;
    for (size_t i = 0; i < nbFaces; i++)
    {
        // Create first triangle
        fsFaces[k] = triangles[i].a[0];
        fsFaces[k+1] = triangles[i].a[1];
        fsFaces[k+2] = triangles[i].a[2];
        k+=3;
    }

    // allocate flat surface materials -- one for each face
//    fsMaterials = new NxMaterialIndex[nbFaces];

    // Build vertex normals
//    fsNormals = new NxVec3[nbFaces];
//    NxBuildSmoothNormals(nbFaces, nbVerts, fsVerts, fsFaces, NULL, fsNormals, true);

    NxTriangleMeshDesc fsDesc;
    fsDesc.numVertices = nbVerts;
    fsDesc.numTriangles = nbFaces;
    fsDesc.pointStrideBytes = sizeof(NxVec3);
    fsDesc.triangleStrideBytes = 3*sizeof(NxU32);
    fsDesc.points = fsVerts;
    fsDesc.triangles = fsFaces;       
    fsDesc.flags = 0;

    // Add the mesh material data
//    fsDesc.materialIndexStride = sizeof(NxMaterialIndex);
//    fsDesc.materialIndices = fsMaterials;

    NxTriangleMeshShapeDesc fsShapeDesc;

    NxInitCooking();

    // Cooking from memory
    MemoryWriteBuffer buf;
    bool status = NxCookTriangleMesh(fsDesc, buf);
    fsShapeDesc.meshData = physicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));

    if (fsShapeDesc.meshData)
    {
        NxActorDesc actorDesc;
        actorDesc.shapes.pushBack(&fsShapeDesc);
        actorDesc.globalPose.t = NxVec3(0,0,0);
        NxActor* actor = scene->createActor(actorDesc);

        // Attach drawable mesh to shape's user data
        NxShape*const* shapes = actor->getShapes();
//        shapes[0]->isTriangleMesh()->getTriangleMesh().saveToDesc(tmd);
//        shapes[0]->userData = (void*)&tmd;

        return actor;
//        gPhysicsSDK->releaseTriangleMesh(*fsShapeDesc.meshData);
    }

    return NULL;
}

bool GameAgeiatech::init(Config& _conf, Input& _input)
{
	if (!GamePS::init(_conf, _input))
	{
		return false;
	}
	if (!(scene = initAgeia(global_gravity)))
	{
		abort_init("PhysX", "Cannot init PhysX");
		return false;
	}

    GraphObjectVisitor visitor(*this);

    world->traverse(visitor, ObjectPosition::getIdentity());

	return true;
}

void GameAgeiatech::process()
{
	GamePS::process();

	if (timeInfo.frameLength > EPSILON)
	{
		scene->simulate(timeInfo.frameLength*speedup);
		scene->flushStream();
		scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}
}

void GameAgeiatech::handleEventKeyDown(const std::string& key)
{
	if (key == "g")
	{
        if (boundingModel != NULL)
        {
            std::ofstream output("../particles.log");

            ParticleCalculator visitor(*boundingModel, output);

            visitor.clear();
            world->traverse(visitor, ObjectPosition::getIdentity());

            crosses = visitor.cnt;

            output << "Total particle count: " << visitor.cnt << std::endl;
            output << "Total particle volume as sphere: " << visitor.volume << std::endl;
            if (dynamic_cast<GraphObjectBox*>(boundingModel) != NULL)
            {
                GraphObjectBox* box = dynamic_cast<GraphObjectBox*>(boundingModel);
                v3 box_size = box->getSize();
                float box_volume = box_size.x*box_size.y*box_size.z;
                output << "Bounding box volume: " << box_volume << std::endl;
                output << "Free space: " << box_volume - visitor.volume << std::endl;
            }
        }
	}
	if (key == ";")
	{
		//this is a trigger to drop particle coords into file 
		//see at particle_calculator.cpp; // void ParticleCalculator::postvisit(IN OUT Particle* particle)
	}
    else
    {
        GamePS::handleEventKeyDown(key);
    }
}


GameAgeiatech::~GameAgeiatech()
{
	exitAgeia();
}

