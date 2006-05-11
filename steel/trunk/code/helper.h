// Helper for Physic Engine
// draws velocity, collisions

#ifndef __HELPER_H
#define __HELPER_H

#include "_cpp.h"
#include "math/vector3d.h"
#include "math/vector4d.h"
#include "math/line.h"

/*
Этот класс предназначен для вывода отладочной информации на экран
стандартными средствами графического движка.
Этот класс умеет рисовать линии, стрелки, выводить текст.
Хранит в себе всю переданную информацию до тех пор,
пока не истечёт время. То есть, стрелка может храниться секунду, а
потом плавно расствориться.
*/
class EngineHelper // Abstract Interface
{
public:
	virtual void clean() = 0;	
	virtual void setTime(const steel::time _time) = 0;
	virtual void drawLine(const Line line, const steel::time duration, const steel::time fade, const v4 color) = 0;
	virtual void drawVector(const Line line, const steel::time duration, const steel::time fade, const v4 color) = 0;
};

#endif
