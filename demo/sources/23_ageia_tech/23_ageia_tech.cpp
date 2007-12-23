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
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

#include <NxPhysics.h>
#include "error_stream.h"

NxScene* gScene = NULL;

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
	sceneDesc.gravity				= NxVec3(0.0f, 0.0f, -9.81f);
	gScene = scene = physicsSDK->createScene(sceneDesc);
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

	// Create ground plane
	NxPlaneShapeDesc planeDesc;
	planeDesc.normal = NxVec3(0.0f, 0.0f, 1.0f);
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	scene->createActor(actorDesc);

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
			physicsSDK->releaseScene(*gScene);
		}
		gScene = scene = NULL;
		NxReleasePhysicsSDK(physicsSDK);
		physicsSDK = NULL;

		log_msg("ageia", "Ageia exited");
	}
}


GameAgeiatech::GameAgeiatech() :
	physicsSDK(NULL),
	scene(NULL)
{}

bool GameAgeiatech::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	if (!initAgeia())
	{
		abort_init("ageia", "Cannot init Ageia");
		return false;
	}

	return true;
}

void GameAgeiatech::process()
{
	GameFreeScene::process();

}

void GameAgeiatech::handleEventKeyDown(const std::string& key)
{
    GameFreeScene::handleEventKeyDown(key);
}

std::string GameAgeiatech::getWindowCaption()
{
    return  "Ageiatech FPS " + graphTimer.getfps_s();
}

