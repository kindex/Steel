/*id*********************************************************
	File: math/sprite.h
	Unit: math
	Part of: Steel engine
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Sprite align calculations
************************************************************/

#ifndef MATH_SPRITE_H
#define MATH_SPRITE_H

#include "vector3d.h"

enum SpriteAlign
{
	SPRITE_ALIGN_CAMERA,
	SPRITE_ALIGN_Z,
	SPRITE_ALIGN_SCREEN,
	SPRITE_ALIGN_CUSTOM
};

class Camera;

void calculateSprite(const Camera& camera,
					const v3 spritePosition,
					const float spriteSize,
					const SpriteAlign align,
					const v3 customAlign,
					OUT v3& vertex1,
					OUT v3& vertex2,
					OUT v3& vertex3,
					OUT v3& vertex4,
					OUT v3& normal);

#endif
