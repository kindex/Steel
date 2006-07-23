/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
        Steel Engine License
    Description:
		Helper for Physic Engine
		draws velocity, collisions
		���� ����� ������������ ��� ������ ���������� ���������� �� �����
		������������ ���������� ������������ ������.
		���� ����� ����� �������� �����, �������, �������� �����.
		������ � ���� ��� ���������� ���������� �� ��� ���,
		���� �� ������� �����. �� ����, ������� ����� ��������� �������, �
		����� ������ �������������.
 ************************************************************/

#ifndef __HELPER_H
#define __HELPER_H

#include "steel.h"
#include "_cpp.h"

#include "graph/types.h"

#include "math/vector3d.h"
#include "math/line.h"
#include "math/aabb.h"

class EngineHelper: public steelAbstract // Abstract Interface
{
public:
	virtual void clean() = 0;	
	virtual void setTime(const steel::time _time) = 0;
	virtual void drawLine(const Line line, const steel::time duration, const steel::time fade, const color4f color) = 0;
	virtual void drawVector(const Line line, const steel::time duration, const steel::time fade, const color4f color) = 0;
	virtual void drawBox(const aabb box, const steel::time duration, const steel::time fade, const color4f color) = 0;
};

#endif
