/*id*********************************************************
	File: res/image/res_image.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Image collection
 ************************************************************/

#ifndef _RES_IMAGE_H
#define _RES_IMAGE_H
#include <string>
class Image;

Image* createImage(const std::string& filename, const std::string& base);


#endif
