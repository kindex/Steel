/*id*********************************************************
	File: res/image/bmp.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#ifndef __RES_IMAGE_BMP_H
#define __RES_IMAGE_BMP_H

#include "../../steel.h"
#include "image.h"

class BMP: public Image
{
public:
	bool init(const std::string name);
};

Image* createBMP(const std::string filename);


#endif
