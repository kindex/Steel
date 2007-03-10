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
//� ����������� �� ���� ������������ ������������ ������������� � ��������� ������� (dir).
//cameraDirection � ��� ������
//cameraEye � ��������� ������
//pos � ��������� �������
	switch (align)
	{
		case SPRITE_ALIGN_SCREEN:	dir = -camera.getDirection(); break;
		case SPRITE_ALIGN_CAMERA:	dir = camera.getPosition() - spritePosition; break;
		case SPRITE_ALIGN_Z:		dir = camera.getPosition() - spritePosition; dir.z = 0; break;
		case SPRITE_ALIGN_CUSTOM:	dir = customAlign; break;
	}

	dir.normalize();
	// per1 � per2 - ��� ������������ ������ �������
	v3 per1(-dir.y, dir.x, 0); // ������������ � dir
	per1.normalize();
	v3 per2 = dir.crossProduct(per1); // ������������� � dir � per1
	per1 *= spriteSize; // �������� �� ������ �������
	per2 *= spriteSize;

	// ���� �������
	vertex1  = spritePosition + per1 - per2;
	vertex2  = spritePosition + per1 + per2;
	vertex3  = spritePosition + -per1 + per2;
	vertex4  = spritePosition + -per1 - per2;

	normal = dir;
}
