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

#include "../../steel.h"
#include "res_image.h"
#include "bmp.h"
#include "tga.h"


// Image filename:
// name[.cube][.normal|.height|.alpha][.bmp|.tga]

Image* createBitmap(const std::string& filename)
{
	Image *r;
	// try to load image2d
	if ((r = createBMP(filename))!= NULL)
	{
		return r;
	}
	if ((r = createTGA(filename))!= NULL)
	{
		return r;
	}
	return NULL;
}

Image* createImageFormat(const std::string& filename)
{
	Image* r;
	// try to load image2d
	if ((r = createBitmap(filename)) != NULL)
	{
		Image* alpha;
		if ((alpha = createBitmap(filename + ".alpha")) != NULL)
		{
			r->addAlphaChannel(alpha);
			delete alpha;
		}

		return r;
	}
	if ((r = createBitmap(filename + ".normal"))	!= NULL)
	{
		r->setFormat(IMAGE_RGB);
		return r;
	}
	if ((r = createBitmap(filename + ".height")) != NULL)
	{
		r->convertFromHeightMapToNormalMap();
		r->setFormat(IMAGE_RGB);
		return r;
	}

	return NULL;
}


Image* createImage(const std::string& filename, const std::string& base)
{
	std::string file = base + "/" +  filename;
	Image* r;
	// try to load image2d
	if ((r = createImageFormat(file)) != NULL)
	{
		return r;
	}
	// try to load cubemap (6 images in one file)
	if ((r = createImageFormat(file + ".cube")) != NULL)
	{
		r->setDimension(IMAGE_CUBE);
		return r;
	}
	
	// try to load cubemap (6 images in one file)
	if ((r = createImageFormat(file + ".forward")) != NULL)
	{
		r->rotate90();
		Image* r2 = createImageFormat(file + ".back");	r2->rotate270(); r->append(r2); delete r2;
		Image* r3 = createImageFormat(file + ".left");	r->append(r3); delete r3;
		Image* r4 = createImageFormat(file + ".right");	r2->rotate180(); r->append(r4); delete r4;
		Image* r5 = createImageFormat(file + ".up");	r5->rotate90(); r->append(r5); delete r5;
		Image* r6 = createImageFormat(file + ".down");	r6->rotate90(); r->append(r6); delete r6;
		r->setDimension(IMAGE_CUBE);
		return r;
	}

	return NULL;
}
