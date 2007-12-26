/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Game main unit
 ************************************************************/

#include "20_labyrinth.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>
#include <math/plane.h>

#include <NxPhysics.h>
#include <NxCooking.h>
#include <Stream.h>
#include "../23_ageia_tech/error_stream.h"

struct CharacterCollector : public Visitor
{
    bool visit(IN OUT GameObject* object)
    {
        if (dynamic_cast<Character*>(object) != NULL)
        {
            characters.push_back(dynamic_cast<Character*>(object));
        }
        return true;
    }

    std::vector<Character*> characters;
};

class AgeiaInjector : public Visitor
{
public:
    AgeiaInjector(GameLabyrinth& game) : 
        game(game),
        inChar(0)
    {}

private:
    bool visit(IN OUT GameObject* object)
    {
        bool is_char = dynamic_cast<Character*>(object) != NULL;
        if (is_char || inChar > 0)
        {
            inChar++;
        }

        return true;
    }

    void postvisit(IN OUT GameObject* object, const ObjectPosition& base_position)
    {
        if (dynamic_cast<GraphObject*>(object) != NULL)
        {
            NxActor* actor = game.createSurface(*dynamic_cast<GraphObject*>(object), base_position, inChar == 0);
            if (inChar != 0)
            {
                actor->userData = (void*)(1);
                game.character->physic_object = actor;
            }
        }
        if (inChar > 0)
        {
            inChar--;
        }
    }

    GameLabyrinth& game;
    int inChar;
};

GameLabyrinth::GameLabyrinth():
    character(NULL)
{}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	cameraMode = C_FIXED;
	std::string dirs[] = {"x", "y"};

	for (int i = 0; i < 2; i++)
	{
		const std::string& dir = dirs[i];
		length[i] = _conf.getf("labyrinth.length_" + dir, 1.0f);
		count[i] = _conf.geti("labyrinth.count_" + dir, 8);
		Config* loadedWallConfig = _conf.find("labyrinth.scene_" + dir);
		if (loadedWallConfig == NULL)
		{
			abort_init("error game res", "Cannot find scene_" + dir + " config");
			return false;
		}
		if (loadedWallConfig->getType() != CONFIG_VALUE_ARRAY)
		{
			abort_init("error game res", "scene_" + dir + " is not array");
		}

		wscene[i] = static_cast<ConfigArray*>(loadedWallConfig);
	}

	labyrinth = generateLabyrinth(count[0], count[1]);

	for (int i = -1; i < labyrinth.getMaxX(); i++)
	{
		for (int j = -1; j < labyrinth.getMaxY(); j++)
		{
			bool right = labyrinth.isRightBorder(i, j);

			if (right && i < labyrinth.getMaxX() && j >= 0)
			{
				Config* currentWallConfig = wscene[0]->getArrayElement(irand(wscene[0]->size()));

				currentWallConfig->setValued("origin[0]", (i + 0.5f)*length[0]);
				currentWallConfig->setValued("origin[1]", j*length[1]);
				GameObject* wall = createGameObject(currentWallConfig);

				world->addObject(wall);
			}

			bool down = labyrinth.isDownBorder(i, j);
			if (down && j < labyrinth.getMaxY() && i >= 0)
			{
				Config* currentWallConfig = wscene[1]->getArrayElement(irand(wscene[1]->size()));

				currentWallConfig->setValued("origin[0]", i*length[0]);
				currentWallConfig->setValued("origin[1]", (j + 0.5f)*length[1]);
				GameObject* wall = createGameObject(currentWallConfig);

                world->addObject(wall);
			}
		}
	}

    CharacterCollector characterCollector;
    world->traverse(characterCollector, ObjectPosition::getIdentity());

    if (characterCollector.characters.empty())
    {
        abort_init("labyrinth", "No character was found");
    }
    character = characterCollector.characters[0];
    character->input = input;

// Physic
    global_gravity = _conf.getv3("physic.global_gravity", v3(0.0f, 0.0f, -9.8f));

	if (!initAgeia())
	{
		abort_init("ageia", "Cannot init Ageia");
		return false;
	}

    AgeiaInjector visitor(*this);
    world->traverse(visitor, ObjectPosition::getIdentity());

    // Ground Plane
    {
	    NxPlaneShapeDesc planeDesc;
	    planeDesc.normal = NxVec3(0.0f, 0.0f, 1.0f);
	    NxActorDesc actorDesc;
	    actorDesc.shapes.pushBack(&planeDesc);
	    pScene->createActor(actorDesc);
    }

	return true;
}

void GameLabyrinth::process()
{
	GameFreeScene::process();

	if (timeInfo.frameLength > EPSILON)
	{
		pScene->simulate(timeInfo.frameLength);
		pScene->flushStream();
		pScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}
}

void GameLabyrinth::handleEventKeyDown(const std::string& key)
{
	if (key == "return")
	{
		if (cameraMode == C_FIXED)
		{
			cameraMode = C_FREE;
		}
		else
		{
			cameraMode = C_FIXED;
		}
	}
    GameFreeScene::handleEventKeyDown(key);
}

std::string GameLabyrinth::getWindowCaption()
{
    return  "Labyrinth FPS " + graphTimer.getfps_s()
			+ " Batches: " + IntToStr(graphEngine->total.batchCount)
			+ " Faces: " + IntToStr(graphEngine->total.triangleCount);
}

void GameLabyrinth::draw(GraphEngine& graph)
{
	// Render all actors
	int nbActors = pScene->getNbActors();
	NxActor** actors = pScene->getActors();
	while (nbActors--)
	{
		NxActor* actor = *actors++;
		if (!actor->userData)
		{
			continue;
		}

		float glMat[16];
		actor->getGlobalPose().getColumnMajor44(glMat);
		v3 zero;
		zero.loadZero();
		matrix34 m;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m.data.matrix.data.a[i * 3 + j] = glMat[i * 4 + j];
			}
		}

        character->position.setTranslation(v3(glMat[3 * 4 + 0], glMat[3 * 4 + 1], glMat[3 * 4 + 2]));
	}

    if (cameraMode == C_FIXED)
    {
        v3 dir = v3(2, 0, -1);
        spectator.camera.setUpVector(v3(0,0,1));
        spectator.camera.setDirection(dir);
        spectator.camera.setPosition(character->getPosition().getTranslation() - dir);
        info.camera = spectator.camera;
    }
    
	GameFreeScene::draw(graph);
}

bool GameLabyrinth::initAgeia()
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

	physicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

	// Create a scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3(global_gravity.x, global_gravity.y, global_gravity.z);
	pScene = physicsSDK->createScene(sceneDesc);
	if (pScene == NULL) 
	{
		error("ageia", "Unable to create a PhysX scene");
		return false;
	}

	// Set default material
	NxMaterial* defaultMaterial = pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	log_msg("ageia", "Ageia connected");

	return true;
}
void GameLabyrinth::exitAgeia()
{
	if (physicsSDK != NULL)
	{
		if (pScene != NULL)
		{
			physicsSDK->releaseScene(*pScene);
		}
		pScene = NULL;
		NxReleasePhysicsSDK(physicsSDK);
		physicsSDK = NULL;

		log_msg("ageia", "Ageia exited");
	}
}

NxActor* GameLabyrinth::createSurface(const GraphObject& object, const ObjectPosition& position, bool _static)
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

    MemoryWriteBuffer buf;
    bool status = NxCookTriangleMesh(fsDesc, buf);
    fsShapeDesc.meshData = physicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));

    if (fsShapeDesc.meshData)
    {
	    NxBodyDesc bodyDesc;
	    bodyDesc.angularDamping	= 0.5f;

        NxActorDesc actorDesc;
        actorDesc.shapes.pushBack(&fsShapeDesc);
        if (!_static)
        {
            actorDesc.body			= &bodyDesc;
        }
        for (int i = 0; i < 3; i++)
        {
            v3 row = position.getRow(i);
            actorDesc.globalPose.M.setRow(i, NxVec3(row.x, row.y, row.z));
        }
        v3 pos = position.getTranslation();
        actorDesc.globalPose.t  = NxVec3(pos.x, pos.y, pos.z);

    	actorDesc.density		= 10.0f;

        NxActor* actor = pScene->createActor(actorDesc);
        return actor;
    }

    return NULL;
}

void Character::traverse(Visitor& visitor, const ObjectPosition& base_position)
{
    if (visitor.visit(this))
    {
        ObjectPosition new_position = position*base_position;

        if (graph_object != NULL)
        {
            graph_object->traverse(visitor, new_position);
        }
        visitor.postvisit(this, new_position);
    }
}
