/*id*********************************************************
	File: res/image/image.cpp
	Unit: res/image
	Part of: Steel engine Res unit
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс для хранения изображения в виде RGB (без загрузки)
 ************************************************************/

#include "../../steel.h"
#include "image.h"
#include "../../math/maths.h"

bool Image::createImage(int WIDTH, int HEIGHT, int BPP)
{
	if (BPP != 24 && BPP != 32) return false; // Only RGB or RGBA
    this->width = WIDTH;
    this->height = HEIGHT;
    this->bpp = BPP;
	this->bitmapSize = this->bpp/8*this->width*this->height;
    this->bitmap = new unsigned char[this->bitmapSize];
    return this->bitmap != NULL;
}

void Image::flipV()
{
    if (bitmap == NULL) return ;

    int bpl = width*3;
    unsigned char *a = (unsigned char*)malloc(bpl); // RGB

    unsigned int d = 0, u = height-1;
    while (d<u)
    {
        memcpy(a, bitmap+bpl*u, bpl);
        memcpy(bitmap+bpl*u, bitmap+bpl*d,bpl);
        memcpy(bitmap+bpl*d, a,bpl);
        d++;
        u--;
    }
    free(a);
}


void Image::flipH()
{
  if (bitmap == NULL) return ;
  int bpl = width*3;

  for (size_t y = 0; y<height; y++)
    for (size_t x = 0; x<(width-1)/2; x++)
    {
        unsigned char a = bitmap[y*bpl + x*3 + 0];
        unsigned char b = bitmap[y*bpl + x*3 + 1];
        unsigned char c = bitmap[y*bpl + x*3 + 2];

        bitmap[y*bpl + x*3 + 0] = bitmap[y*bpl + (width-x-1)*3 + 0];
        bitmap[y*bpl + x*3 + 1] = bitmap[y*bpl + (width-x-1)*3 + 1];
        bitmap[y*bpl + x*3 + 2] = bitmap[y*bpl + (width-x-1)*3 + 2];

        bitmap[y*bpl + (width-x-1)*3 + 0] = a;
        bitmap[y*bpl + (width-x-1)*3 + 1] = b;
        bitmap[y*bpl + (width-x-1)*3 + 2] = c;
    }
}

void Image::putpixel(float X, float Y, float r, float g, float b)
{
    if (bitmap == NULL) return ;
    size_t x = (int)(X*width);
    if (x<0 || x>= width) return;

    size_t y = (int)(Y*height);
    if (y<0 || y>= height) return;

    size_t bpl = width*3;
    bitmap[y*bpl + x*3 + 0] = (int)(r*255);
    bitmap[y*bpl + x*3 + 1] = (int)(g*255);
    bitmap[y*bpl + x*3 + 2] = (int)(b*255);
}

void Image::putpixeladd(float X, float Y, float r, float g, float b)
{
    if (bitmap == NULL) return ;
    size_t x = (int)(X*width);
    if (x<0 || x>= width) return;

    size_t y = (int)(Y*height);
    if (y<0 || y>= height) return;

    size_t bpl = width*3;
    int a;
    a = (int)bitmap[y*bpl + x*3 + 0] + (int)(r*255); if (a<0) a = 0; if (a>255) a = 255; bitmap[y*bpl + x*3 + 0] = a;
    a = (int)bitmap[y*bpl + x*3 + 1] + (int)(g*255); if (a<0) a = 0; if (a>255) a = 255; bitmap[y*bpl + x*3 + 1] = a;
    a = (int)bitmap[y*bpl + x*3 + 2] + (int)(b*255); if (a<0) a = 0; if (a>255) a = 255; bitmap[y*bpl + x*3 + 2] = a;
}


void Image::putpixelalfa(float X, float Y, float r, float g, float b, float alfa)
{
    if (bitmap == NULL) return ;
    size_t x = (int)(X*width);
    if (x<0 || x>= width) return;

    size_t y = (int)(Y*height);
    if (y<0 || y>= height) return;

    size_t bpl = width*3;
    bitmap[y*bpl + x*3 + 0] = (unsigned char)(bitmap[y*bpl + x*3 + 0]*(1-alfa) + alfa*r*255);
    bitmap[y*bpl + x*3 + 1] = (unsigned char)(bitmap[y*bpl + x*3 + 1]*(1-alfa) + alfa*g*255);
    bitmap[y*bpl + x*3 + 2] = (unsigned char)(bitmap[y*bpl + x*3 + 2]*(1-alfa) + alfa*b*255);
}


void Image::clear(float r, float g, float b)
{
    if (bitmap == NULL) return;
    size_t bpl = width*3;
    for (size_t y=0; y<height; y++)
	{
        for (size_t x=0; x<width; x++)
        {
            bitmap[y*bpl + x*3 + 0] = (int)(r*255);
            bitmap[y*bpl + x*3 + 1] = (int)(g*255);
            bitmap[y*bpl + x*3 + 2] = (int)(b*255);
        }
	}
}

void Image::convertFromHeightMapToNormalMap()
{
    int bpl = width*3;
    unsigned char *a = (unsigned char*)malloc(bpl); // RGB
    unsigned char *b = (unsigned char*)malloc(bpl); // RGB

    memcpy(b, &bitmap[(height-1)*bpl], bpl);

    for (size_t y=0; y<height; y++)
    {
        memcpy(a, b, bpl);
        memcpy(b, &bitmap[y*bpl], bpl);

        for (size_t x=0; x<width; x++)
        {
            unsigned char A = a[x*3];
//            unsigned char B = b[x*3];
            unsigned char C = b[(x+width-1)%width*3];
            unsigned char D = b[(x+1)%width*3];
            unsigned char E = bitmap[bpl*(y+1)%height+x*3];

            float X, Y, Z;

            X = ((float)C-(float)D)/255.0f;
            Y = ((float)E- (float)D)/255.0f;

           //Z = 1 - sqr(X) -sqr(Y);
			Z = sqrt(1 - X*X - Y*Y);

            float r = clamp((X + 1.0f)/2);
            float g = clamp((Y + 1.0f)/2);
            float b = clamp((Z + 1.0f)/2);

            bitmap[bpl*y + x*3 + 0] = (unsigned char)(r*255);
            bitmap[bpl*y + x*3 + 1] = (unsigned char)(g*255);
            bitmap[bpl*y + x*3 + 2] = (unsigned char)(b*255);
        }
    }
    free(a);
    free(b);

	format = IMAGE_RGB;
}

bool Image::unload()
{
	if (bitmap != NULL)
	{
		delete[] bitmap;
	}
	bitmap = NULL;
	bpp = 0; width = 0; height = 0; bitmapSize = 0;
	return true;
 }

bool Image::convertToRGBA()
{
	if (getFormat() == IMAGE_RGB)
	{
		format = IMAGE_RGBA;
		bpp = 32;
		bitmapSize = width*height*bpp/8;

		unsigned char* newBitmap = new unsigned char[this->bitmapSize];

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				newBitmap[(y*width+x)*4 + 0] = bitmap[(y*width+x)*3 + 0];
				newBitmap[(y*width+x)*4 + 1] = bitmap[(y*width+x)*3 + 1];
				newBitmap[(y*width+x)*4 + 2] = bitmap[(y*width+x)*3 + 2];
				newBitmap[(y*width+x)*4 + 3] = 1;
			}
		}

		delete[] bitmap;
		bitmap = newBitmap;
		return true;
	}
	else
		return false;
}


void Image::addAlphaChannel(const Image* alpha)
{
	convertToRGBA();
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			bitmap[(y*width+x)*4 + 3] = 
				(alpha->bitmap[(y*width+x)*alpha->bpp/8 + 0] +
				 alpha->bitmap[(y*width+x)*alpha->bpp/8 + 1] +
				 alpha->bitmap[(y*width+x)*alpha->bpp/8 + 2]) / 3;
		}
	}
}
