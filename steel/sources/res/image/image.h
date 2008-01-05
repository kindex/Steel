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

enum ImageDimension
{
	IMAGE_DIMENSION_NONE,
	IMAGE_1D,
	IMAGE_2D,
	IMAGE_3D,
	IMAGE_CUBE // 6 images in one (+X, -X, +Y, -Y, +Z, -Z)
};

enum ImageFormat
{
	IMAGE_FORMAT_NONE,
	IMAGE_RGB,   // 24
	IMAGE_RGBA  // 32
} ;

class Image: public Res
{
public:
	Image();
    ~Image();
    std::string getPath() const;

    void unload();
    bool createImage(int WIDTH, int HEIGHT, int BPP);

    void flipV(); // perevernut' Vertikalno |
    void flipH(); // perevernut' <->

    void putpixel		(float x, float y, float r, float g, float b);
    void putpixelalfa	(float x, float y, float r, float g, float b, float alfa);
    void putpixeladd	(float x, float y, float r, float g, float b);
    void clear			(float r, float g, float b);

	int getWidth()	const { return width; }
	int getHeight()	const { return height; }
	int getBpp()	const { return bpp; }
	const unsigned char* getBitmap() const { return bitmap; }

	void convertFromHeightMapToNormalMap();

	ImageDimension getDimension() const { return dimension; }
	void setDimension(ImageDimension _dimension) { dimension = _dimension; }
	ImageFormat	getFormat() const { return format; }
	void setFormat(ImageFormat _format) { format = _format; }
	void addAlphaChannel(const Image* alpha);
	bool convertToRGBA();
	void append(const Image* end);
	void resize(size_t newWidth, size_t newHeight);
	void rotate90();
	void rotate180();
	void rotate270();

protected:
    unsigned char* bitmap;
    size_t width;
	size_t height;
	size_t bpp; // bits per pixel
	size_t bitmapSize;

	ImageDimension	dimension;
	ImageFormat		format;
    std::string     path;
};

#endif
