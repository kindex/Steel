/*id*********************************************************
    Unit: Res-Image [Resources - Images - BMP (RGB bitmap)]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#ifndef __RES_IMAGE_BMP_H
#define __RES_IMAGE_BMP_H

#include "image.h"

class BMP: public Image
{
public:
	bool init(const std::string name, ResCollection &res);
	bool unload();
};

#endif
