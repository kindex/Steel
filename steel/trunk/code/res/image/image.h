/*id*********************************************************
    Unit: Res-Image [Resources - Images (RGB bitmap)]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс для хранения изображения в виде RGB (без загрузки)
 ************************************************************/

#ifndef __RES_IMAGE_H
#define __RES_IMAGE_H

#include "../res.h"

class NormalMap;
class HeightMap;

class Image: public Res
{
protected:
    unsigned char* bitmap;
    int width, height, bpp, bitmapSize;
public:
    Image(): Res() { bpp = 0; width = 0; height = 0; bitmap = NULL; bitmapSize = 0; }
    ~Image() { }

    bool unload() 
	{
		if (bitmap != NULL)
			delete bitmap;
		bitmap = NULL;
		bpp = 0; width = 0; height = 0; bitmapSize = 0;
		return true;
     }

    bool init(int WIDTH, int HEIGHT, int BPP);

    void flipV(); // perevernut' Vertikalno |
    void flipH(); // perevernut' <->

    void putpixel		(float x, float y, float r, float g, float b);
    void putpixelalfa	(float x, float y, float r, float g, float b, float alfa);
    void putpixeladd	(float x, float y, float r, float g, float b);
    void clear			(float r, float g, float b);

	int getWidth()	{ return width; }
	int getHeight()	{ return height; }
	unsigned char* getBitmap() { return bitmap; }

	void convertFromHeightMapToNormalMap();
};

#endif
