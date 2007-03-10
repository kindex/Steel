/*id*********************************************************
	File: math/sprite.cpp
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

#include "sprite.h"
#include "../common/types.h"
#include "../engine/engine_types.h"

void calculateSprite(const Camera& camera,
					const v3 spritePosition,
					const float spriteSize,
					const SpriteAlign align,
					const v3 customAlign,
					OUT v3& vertex1,
					OUT v3& vertex2,
					OUT v3& vertex3,
					OUT v3& vertex4,
					OUT v3& normal)
{
	v3 dir;
//¬ зависимости от типа выравнивани€ рассчитываем перпендикул€р к плоскости спрайта (dir).
//cameraDirection Ц ось камеры
//cameraEye Ц положение камеры
//pos Ц положение спрайта
	switch (align)
	{
		case SPRITE_ALIGN_SCREEN:	dir = -camera.getDirection(); break;
		case SPRITE_ALIGN_CAMERA:	dir = camera.getPosition() - spritePosition; break;
		case SPRITE_ALIGN_Z:		dir = camera.getPosition() - spritePosition; dir.z = 0; break;
		case SPRITE_ALIGN_CUSTOM:	dir = customAlign; break;
	}

	dir.normalize();
	// per1 и per2 - это направл€ющие сторон спрайта
	v3 per1(-dir.y, dir.x, 0); // перендикул€р к dir
	per1.normalize();
	v3 per2 = dir.crossProduct(per1); // перпендикул€р к dir и per1
	per1 *= spriteSize; // умножаем на размер спарйта
	per2 *= spriteSize;

	// углы спарйта
	vertex1  = spritePosition + per1 - per2;
	vertex2  = spritePosition + per1 + per2;
	vertex3  = spritePosition + -per1 + per2;
	vertex4  = spritePosition + -per1 - per2;

	normal = dir;
}
