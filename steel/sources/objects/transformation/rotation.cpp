#include "rotation.h"

bool Rotation::InitFromConfig(Config* conf)
{
	speed = conf->getf("speed", 1.0f);
	rotationAxis = conf->getv3("rotationAxis", v3(0.0f, 0.0f, 1.0f));
	currentPosition.loadIdentity();

	return true;
}

void Rotation::process(IN const ProcessInfo& info)
{
	currentPosition.setRotationAxis(info.curTime*speed, rotationAxis);
}

ObjectPosition Rotation::getPosition()
{
	return currentPosition;
}
