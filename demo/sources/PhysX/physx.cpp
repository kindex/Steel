#include "physx.h"
#include <common/logger.h>

#include <NxPhysics.h>
#include <NxCooking.h>
#include <Stream.h>
#include "error_stream.h"

NxScene* globalScene = NULL;
NxPhysicsSDK* physicsSDK = NULL;

NxScene* initAgeia(const v3& global_gravity)
{
	// Initialize PhysicsSDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, new ErrorStream(), desc, &errorCode);
	if (physicsSDK == NULL) 
	{
		error("PhysX", std::string("SDK create error (") + IntToStr(errorCode) + " - " + getNxSDKCreateError(errorCode) + ").");
		return false;
	}
#if SAMPLES_USE_VRD
	// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

	physicsSDK->setParameter(NX_SKIN_WIDTH, 0.00001f);

	// Create a scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity	= NxVec3(global_gravity.x, global_gravity.y, global_gravity.z);
	globalScene = physicsSDK->createScene(sceneDesc);
	if (globalScene == NULL) 
	{
		error("PhysX", "Unable to create a PhysX scene");
		return NULL;
	}

	// Set default material
	NxMaterial* defaultMaterial = globalScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);

	log_msg("PhysX", "PhysX connected");

	return globalScene;
}

void exitAgeia()
{
	if (physicsSDK != NULL)
	{
		if (globalScene != NULL)
		{
			physicsSDK->releaseScene(*globalScene);
		}
		globalScene = NULL;
		NxReleasePhysicsSDK(physicsSDK);
		physicsSDK = NULL;

		log_msg("PhysX", "PhysX exited");
	}
}

