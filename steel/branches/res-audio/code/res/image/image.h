/*id*********************************************************
    Unit: Res-Image [Resources - Images (RGB bitmap)]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		 ласс дл€ хранени€ изображени€ в виде RGB (без загрузки)
 ************************************************************/

#ifndef __RES_IMAGE_H
#define __RES_IMAGE_H

#include "../res.h"

namespace ImageKind
{
	typedef enum 
	{
		none,
		image1d,
		image2d,
		image3d,
		cube // 6 images in one (+X, -X, +Y, -Y, +Z, -Z)
	} ImageKind;
}

namespace ImageFormat
{
	typedef enum
	{
		none,
		rgb,   // 24
		rgba,  // 32
		normal // rgb = xyz
	} ImageFormat;
}

class Image: public Res
{
protected:
    unsigned char* bitmap;
    int width, height, bpp, bitmapSize;
public:

protected:
	ImageKind::ImageKind		kind;
	ImageFormat::ImageFormat	format;
public:

	Image(): Res() { bpp = 0; width = 0; height = 0; bitmap = NULL; bitmapSize = 0; kind = ImageKind::none; format =ImageFormat::none; }
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

	ImageKind::ImageKind	getKind() { return kind; }
	void setKind(ImageKind::ImageKind _kind) { kind = _kind; }
	ImageFormat::ImageFormat	getFormat() { return format; }
	void setFormat(ImageFormat::ImageFormat _format) { format = _format; }
};

#endif
