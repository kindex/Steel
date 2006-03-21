#ifndef __RES_IMAGE_H
#define __RES_IMAGE_H

/*
  Image unit

  Класс для хранения изобрадения (без загрузки)
*/

#include "../res.h"

//typedef int tex;
//extern tex imageNone,imageNotFound,imageOutOfMemory,imageWrongFormat,imageOutOfRange;


/*
The first element corresponds	to the lower left corner of
the texture image.  Subsequent elements progress left-to-
right	through	the remaining texels in	the lowest row of the
texture image, and then in successively higher rows of the
texture image.  The final element corresponds	to the upper
right	corner of the texture image.

24 bit !!
*/

class Image: public Res
{

protected:
public:// todo
    unsigned char* bitmap;
    int width, height, bpp, bitmapSize;

public:

    Image(): Res() { bpp = 0; width = 0; height = 0; bitmap = NULL; bitmapSize = 0; }
    ~Image() { }

//    virtual void loadraw(const string& name)=0;
    void convertFromHeightMapToNormalMap();

    void makeCubeMap(const std::string& name);

//    void init(const string& name);

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
    void putpixel(float x, float y, float r, float g, float b);
    void putpixelalfa(float x, float y, float r, float g, float b, float alfa);
    void putpixeladd(float x, float y, float r, float g, float b);
    void clear(float r, float g, float b);

 //   void operator=(const Image &A) { memcpy(this, &A, sizeof(Image)); } 
};


class NormalMap: public Image
{
	bool init(const std::string name, ResCollection &res);
};

#endif
