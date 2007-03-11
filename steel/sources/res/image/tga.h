/*id*********************************************************
	File: res/image/tga.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* NeHe Lesson 32 [nehe.gamedev.net, 2001]
	License:
		Steel Engine License
	Description:
		Класс для загрузки TGA-файла в класс Image
 ************************************************************/

#ifndef __RES_IMAGE_TGA_H
#define __RES_IMAGE_TGA_H

#include "../../steel.h"
#include "image.h"

class TGA: public Image
{
public:
	bool init(const std::string& name);
};

Image* createTGA(const std::string& filename);


#endif
