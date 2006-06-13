/*id*********************************************************
    Unit: Res-Image [Resources - Images - BMP (RGB bitmap)]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		����� ��� �������� BMP-����� � ����� Image
 ************************************************************/

#ifndef __RES_IMAGE_BMP_H
#define __RES_IMAGE_BMP_H

#include "image.h"

class BMP: public Image
{
public:
	bool init(const std::string name);
};

Res* createBMP(const std::string filename, ResCollection *res);


#endif
