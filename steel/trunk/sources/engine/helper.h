/*id*********************************************************
	File: engine/helper.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
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
	virtual ~EngineHelper(void) {}
	virtual void clean(void) = 0;	
	virtual void setTime(const steel::time _time) = 0;
	virtual void drawLine(const Line line, const steel::time duration, const steel::time fade, const color4f color) = 0;
	virtual void drawVector(const Line line, const steel::time duration, const steel::time fade, const color4f color) = 0;
	virtual void drawBox(const AABB box, const steel::time duration, const steel::time fade, const color4f color) = 0;
};

#endif
