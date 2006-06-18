/*id*********************************************************
    Unit: Res-Image [Resources - Images - BMP (RGB bitmap)]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* NeHe Lesson 32 [nehe.gamedev.net, 2001]
    License:
        Steel Engine License
    Description:
		Класс для загрузки TGA-файла в класс Image
 ************************************************************/

#ifndef __RES_IMAGE_TGA_H
#define __RES_IMAGE_TGA_H

#include "image.h"

class TGA: public Image
{
public:
	bool init(const std::string name);
};

Res* createTGA(const std::string filename);


#endif
