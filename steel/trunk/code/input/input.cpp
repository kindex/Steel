#include "input.h"

bool Input::isPressed(std::string key)
{
	return keyPressed.find(key) != keyPressed.end() && keyPressed[key];
}
