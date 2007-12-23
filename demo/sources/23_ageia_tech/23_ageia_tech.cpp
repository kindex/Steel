/*id*********************************************************
	Unit: 23 Ageia tech
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system testtype
 ************************************************************/

#include "23_ageia_tech.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

#include <NxPhysics.h>
#include "error_stream.h"


// Physics
static NxPhysicsSDK*	gPhysicsSDK = NULL;
static NxScene*			gScene = NULL;

// Rendering
static NxVec3	gEye(50.0f, 50.0f, 50.0f);
static NxVec3	gDir(-0.6f,-0.2f,-0.7f);
static NxVec3	gViewY;
static int		gMouseX = 0;
static int		gMouseY = 0;

static bool InitNx()
{
	// Initialize PhysicsSDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, new ErrorStream(), desc, &errorCode);
	if (gPhysicsSDK == NULL) 
	{
		error("ageia", std::string("SDK create error (") + IntToStr(errorCode) + " - " + getNxSDKCreateError(errorCode) + ").");
		return false;
	}
#if SAMPLES_USE_VRD
	// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.05f);

	// Create a scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity				= NxVec3(0.0f, -9.81f, 0.0f);
	gScene = gPhysicsSDK->createScene(sceneDesc);
	if (gScene == NULL) 
	{
		error("ageia", "Unable to create a PhysX scene");
		return false;
	}

	// Set default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	// Create ground plane
	NxPlaneShapeDesc planeDesc;
	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&planeDesc);
	gScene->createActor(actorDesc);
	log_msg("ageia", "Ageia connected");

	return true;
}

static void ExitNx()
{
	if (gPhysicsSDK != NULL)
	{
		if (gScene != NULL)
		{
			gPhysicsSDK->releaseScene(*gScene);
		}
		gScene = NULL;
		NxReleasePhysicsSDK(gPhysicsSDK);
		gPhysicsSDK = NULL;
	}
}


GameAgeiatech::GameAgeiatech()
{}

bool GameAgeiatech::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	if (!InitNx())
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

