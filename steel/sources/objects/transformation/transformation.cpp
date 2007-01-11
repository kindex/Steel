#include "transformation.h"
#include "rotation.h"

Transformation* transformationFactory(const std::string& className)
{
	if(className == "rotation") return new Rotation;

	error("objects", std::string("Transformation class '") + className + "' not found");
	return NULL;
}
