/*id*********************************************************
	File: res/image/bmp.cpp
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#include "../../steel.h"
#include "bmp.h"
#include "../../common/logger.h"
#include "../rstream.h"
#include <string> 

Image* createBMP(const std::string filename)
{
	BMP *o = new BMP;
	if(o->init(filename))
	{
		o->setId(objectIdGenerator.genUid());
		return o;
	}
	else
	{
		delete o;
		return NULL;
	}
}

