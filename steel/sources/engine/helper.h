/*id*********************************************************
	File: engine/helper.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Class for debug information output (for Physic Engine)
		draws velocity, collisions
		Этот класс предназначен для вывода отладочной информации на экран
		стандартными средствами графического движка.
		Этот класс умеет рисовать линии, стрелки, выводить текст.
		Хранит в себе всю переданную информацию до тех пор,
		пока не истечёт время. То есть, стрелка может храниться секунду, а
		потом плавно расствориться.
 ************************************************************/

#ifndef __HELPER_H
#define __HELPER_H

#include "../steel.h"

#include "../graph/graph_types.h"
#include "../math/vector3d.h"
#include "../math/line.h"
#include "../math/aabb.h"

class EngineHelper // Abstract Object
{
public:
	virtual ~EngineHelper() {}
	virtual void clean() abstract;
	virtual void setTime(const steel::time _time) abstract;
	virtual void drawLine(const Line line, const steel::time duration, const steel::time fade, const color4f color) abstract;
	virtual void drawVector(const Line line, const steel::time duration, const steel::time fade, const color4f color) abstract;
	virtual void drawBox(const AABB box, const steel::time duration, const steel::time fade, const color4f color) abstract;
};

#endif
