/*id*********************************************************
	File: res/image/image.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс для хранения изображения в виде RGB (без загрузки)
 ************************************************************/

#ifndef __RES_IMAGE_H
#define __RES_IMAGE_H

#include "../../steel.h"
#include "../res.h"

typedef enum 
{
	IMAGE_DIMENSION_NONE,
	IMAGE_1D,
	IMAGE_2D,
	IMAGE_3D,
	IMAGE_CUBE // 6 images in one (+X, -X, +Y, -Y, +Z, -Z)
} ImageDimension;

typedef enum
{
	IMAGE_FORMAT_NONE,
	IMAGE_RGB,   // 24
	IMAGE_RGBA,  // 32
	IMAGE_NORMAL // rgb = xyz
} ImageFormat;

class Image: public Res
{
protected:
    unsigned char* bitmap;
    int width, height, bpp, bitmapSize;
public:

protected:
	ImageDimension	dimension;
	ImageFormat		format;
public:

	Image(): Res() { bpp = 0; width = 0; height = 0; bitmap = NULL; bitmapSize = 0; dimension = IMAGE_DIMENSION_NONE; format = IMAGE_FORMAT_NONE; }
    ~Image() { unload(); }

    bool unload()
	{
		if (bitmap != NULL)
			delete bitmap;
		bitmap = NULL;
		bpp = 0; width = 0; height = 0; bitmapSize = 0;
		return true;
     }

    bool createImage(int WIDTH, int HEIGHT, int BPP);

    void flipV(); // perevernut' Vertikalno |
    void flipH(); // perevernut' <->

    void putpixel		(float x, float y, float r, float g, float b);
    void putpixelalfa	(float x, float y, float r, float g, float b, float alfa);
    void putpixeladd	(float x, float y, float r, float g, float b);
    void clear			(float r, float g, float b);

	int getWidth()	{ return width; }
	int getHeight()	{ return height; }
	int getBpp()	{ return bpp; }
	unsigned char* getBitmap() { return bitmap; }

	void convertFromHeightMapToNormalMap();

	ImageDimension	getDimension() { return dimension; }
	void setDimension(ImageDimension _dimension) { dimension = _dimension; }
	ImageFormat	getFormat() { return format; }
	void setFormat(ImageFormat _format) { format = _format; }
};

#endif
