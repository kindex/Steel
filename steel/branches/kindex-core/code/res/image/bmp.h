#ifndef __RES_IMAGE_BMP_H
#define __RES_IMAGE_BMP_H

#include "image.h"

class BMP: public Image
{
	bool init(std::string& name);
	bool unload();
};

#endif