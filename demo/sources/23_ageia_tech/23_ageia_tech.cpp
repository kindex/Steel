/*id*********************************************************
	Unit: 23 Ageia tech
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Ageia test
 ************************************************************/

#include "23_ageia_tech.h"
#include "../19_ps/particle_calculator.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

#include <NxPhysics.h>
#include <NxCooking.h>
#include <Stream.h>
#include "error_stream.h"

NxScene* globalScene = NULL;


//class GraphObjectVisitor : public Visitor
//{
//public:
//    ParticleCalculator(GraphObject* graphObject);
//    void clear();
//    void postvisit(IN OUT Particle* particle);
//
//    int cnt;
//    GraphObject* graphObject;
//};

GameAgeiatech::GameAgeiatech() :
	physicsSDK(NULL),
	scene(NULL)
{}

NxActor* GameAgeiatech::createSurface(const GraphObject& object, 
                                      NxTriangleMeshDesc& tmd)
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
        fsVerts[i] = NxVec3(vertexes[i].x, vertexes[i].y, vertexes[i].z); 
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
        shapes[0]->isTriangleMesh()->getTriangleMesh().saveToDesc(tmd);
        shapes[0]->userData = (void*)&tmd;

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
	if (!initAgeia())
	{
		abort_init("ageia", "Cannot init Ageia");
		return false;
	}

	// Create ground plane
	//NxPlaneShapeDesc planeDesc;
	//planeDesc.normal = NxVec3(0.0f, 0.0f, 1.0f);
	//NxActorDesc actorDesc;
	//actorDesc.shapes.pushBack(&planeDesc);
	//scene->createActor(actorDesc);

//    GraphObjectVisitor

    if (boundingModel != NULL)
    {
        NxTriangleMeshDesc tmd;
        NxActor* actor = createSurface(*boundingModel, tmd);
    }

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
		//v3 current = physicEngine->getGravity();
		//physicEngine->setGravity(gravityBackup);
		//gravityBackup = current;
	}
    else
    {
        GameFreeScene::handleEventKeyDown(key);
    }
}

bool GameAgeiatech::initAgeia()
{
	// Initialize PhysicsSDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, new ErrorStream(), desc, &errorCode);
	if (physicsSDK == NULL) 
	{
		error("ageia", std::string("SDK create error (") + IntToStr(errorCode) + " - " + getNxSDKCreateError(errorCode) + ").");
		return false;
	}
#if SAMPLES_USE_VRD
	// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

	physicsSDK->setParameter(NX_SKIN_WIDTH, 0.05f);

	// Create a scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity	= NxVec3(global_gravity.x, global_gravity.y, global_gravity.z);
	globalScene = scene = physicsSDK->createScene(sceneDesc);
	if (scene == NULL) 
	{
		error("ageia", "Unable to create a PhysX scene");
		return false;
	}

	// Set default material
	NxMaterial* defaultMaterial = scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	log_msg("ageia", "Ageia connected");

	return true;
}

GameAgeiatech::~GameAgeiatech()
{
	exitAgeia();
}

void GameAgeiatech::exitAgeia()
{
	if (physicsSDK != NULL)
	{
		if (scene != NULL)
		{
			physicsSDK->releaseScene(*scene);
		}
		globalScene = scene = NULL;
		NxReleasePhysicsSDK(physicsSDK);
		physicsSDK = NULL;

		log_msg("ageia", "Ageia exited");
	}
}

