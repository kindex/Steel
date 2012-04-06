#pragma once

#include <steel.h>
#include <math/vector3d.h>

class NxPhysicsSDK;
class NxScene;
class NxActor;
class NxTriangleMeshDesc;

extern NxScene* globalScene;
extern NxPhysicsSDK*	physicsSDK;

NxScene* initAgeia(const v3& global_gravity);
void exitAgeia();
