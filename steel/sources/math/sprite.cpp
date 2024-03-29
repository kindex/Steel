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

void calculateSprite(const Camera&      camera,
				     const v3           spritePosition,
					 const PositionKind position_kind,
					 const v2           spriteSize,
					 const SpriteAlign  align,
					 const v3           customAlign,
					 OUT   v3&          vertex1,
					 OUT   v3&          vertex2,
					 OUT   v3&          vertex3,
					 OUT   v3&          vertex4,
					 OUT   v3&          normal,
					 OUT   v3&          direction1,
					 OUT   v3&          direction2)
{
	v3 dir;
//� ����������� �� ���� ������������ ������������ ������������� � ��������� ������� (dir).
//cameraDirection � ��� ������
//cameraEye � ��������� ������
//pos � ��������� �������
    if (position_kind == POSITION_SCREEN)
    {
        dir = v3(0, 0, 1);
    }
    else
    {
	    switch (align)
	    {
		    case SPRITE_ALIGN_SCREEN:	dir = -camera.getDirection(); break;
		    case SPRITE_ALIGN_CAMERA:	dir = camera.getPosition() - spritePosition; break;
		    case SPRITE_ALIGN_Z:		dir = camera.getPosition() - spritePosition; dir.z = 0; break;
		    case SPRITE_ALIGN_CUSTOM:	dir = customAlign; break;
	    }
    }

	dir.normalize();
	// per1 � per2 - ��� ������������ ������ �������
	v3 per1(-dir.y, dir.x, 0); // ������������ � dir
    if (per1.getSquaredLengthd() < EPSILON2)
    {
        per1.set(1, 0, 0);
    }
	per1.normalize();
	v3 per2 = dir.crossProduct(per1); // ������������� � dir � per1
	per1 *= spriteSize.x; // �������� �� ������ �������
	per2 *= spriteSize.y;

    direction1 = per1;
    direction2 = per2;

	// ���� �������
	vertex1  = spritePosition + per1 - per2;
	vertex2  = spritePosition + per1 + per2;
	vertex3  = spritePosition + -per1 + per2;
	vertex4  = spritePosition + -per1 - per2;

	normal = dir;
}
