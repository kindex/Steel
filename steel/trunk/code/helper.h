// Helper for Physic Engine
// draws velocity, collisions

#ifndef __HELPER_H
#define __HELPER_H

#include "_cpp.h"
#include "math/vector3d.h"
#include "math/line.h"

/*
���� ����� ������������ ��� ������ ���������� ���������� �� �����
������������ ���������� ������������ ������.
���� ����� ����� �������� �����, �������, �������� �����.
������ � ���� ��� ���������� ���������� �� ��� ���,
���� �� ������� �����. �� ����, ������� ����� ��������� �������, �
����� ������ �������������.
*/
class EngineHelper // Abstract Interface
{
public:
	virtual void clean() = 0;	
	virtual void setTime(const steel::time _time) = 0;
	virtual void drawLine(const Line line, const time duration, const time fade) = 0;
	virtual void drawVector(const Line line, const time duration, const time fade) = 0;
};

#endif
