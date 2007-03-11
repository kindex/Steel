/*id*********************************************************
	File: res/image/jpeg.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		JPEG loader (using NV_UTIL lib)
 ************************************************************/

#ifndef __RES_IMAGE_JPEG_H
#define __RES_IMAGE_JPEG_H

#include "../../steel.h"
#include "image.h"

class ResJPEG: public Image
{
public:
	bool init(const std::string& name);
};

Image* createJPEG(const std::string& filename);


#endif
