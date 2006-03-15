#ifndef __RES_IMAGE_BMP_H
#define __RES_IMAGE_BMP_H

#include "image.h"

class BMP: public Image
{
public:
	bool init(const std::string name, ResLocatorArray &loadBefore, ResLocatorArray &loadAfter);
	bool unload();
};

#endif
