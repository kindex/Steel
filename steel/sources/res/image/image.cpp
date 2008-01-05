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

void Image::unload()
{
	delete[] bitmap;
	bitmap = NULL;
	bpp = width = height = bitmapSize = 0;
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

void Image::append(const Image* end)
{
	if (end == NULL || end->getWidth() != this->width || this->bpp != end->getBpp())
	{
		resize(width, height + width);
		return;
	}
	size_t oldHeight = height;
	resize(width, height + end->getHeight());

	memcpy(bitmap + oldHeight*width*bpp/8, end->getBitmap(),  end->getHeight()*width*bpp/8);
}

void Image::resize(size_t newWidth, size_t newHeight)
{
	if (newWidth == width && newHeight > height)
	{
		size_t newBitmapSize = newWidth*newHeight*bpp/8;
	    unsigned char* newBitmap = new unsigned char[newBitmapSize];
		memcpy(newBitmap, bitmap, bitmapSize);
		delete[] bitmap;
		bitmap = newBitmap;
		width = newWidth;
		height = newHeight;
		bitmapSize = newBitmapSize;
	}
}

void Image::rotate270()
{
	if (width != height || bitmap == NULL)
	{
		return;
	}
	size_t m = width - 1;
	for (size_t y = 0; y < height/2; y++)
		for (size_t x = 0; x < width/2; x++)
			for (size_t z = 0; z < bpp/8; z++)
			{
				size_t LT = (x + y*width)*bpp/8 + z;			unsigned char lt = bitmap[LT];
				size_t LB = (y + (m-x)*width)*bpp/8 + z;		unsigned char lb = bitmap[LB];
				size_t RB = (m - x + (m-y)*width)*bpp/8 + z;	unsigned char rb = bitmap[RB];
				size_t RT = (m-y + x*width)*bpp/8 + z;			unsigned char rt = bitmap[RT];

				bitmap[LT] = rt;
				bitmap[LB] = lt;
				bitmap[RB] = lb;
				bitmap[RT] = rb;
			}
}

void Image::rotate90()
{
	if (width != height || bitmap == NULL)
	{
		return;
	}
	size_t m = width-1;
	for (size_t y = 0; y < height/2; y++)
		for (size_t x = 0; x < width/2; x++)
			for (size_t z = 0; z < bpp/8; z++)
			{
				size_t LT = (x + y*width)*bpp/8 + z;			unsigned char lt = bitmap[LT];
				size_t LB = (y + (m-x)*width)*bpp/8 + z;		unsigned char lb = bitmap[LB];
				size_t RB = (m - x + (m-y)*width)*bpp/8 + z;	unsigned char rb = bitmap[RB];
				size_t RT = (m-y + x*width)*bpp/8 + z;			unsigned char rt = bitmap[RT];

				bitmap[LT] = lb;
				bitmap[LB] = rb;
				bitmap[RB] = rt;
				bitmap[RT] = lt;
			}
}

void Image::rotate180()
{
	if (width != height || bitmap == NULL)
	{
		return;
	}
	size_t m = width - 1;
	for (size_t y = 0; y < height/2; y++)
		for (size_t x = 0; x < width/2; x++)
			for (size_t z = 0; z < bpp/8; z++)
			{
				size_t LT = (x + y*width)*bpp/8 + z;			unsigned char lt = bitmap[LT];
				size_t LB = (y + (m-x)*width)*bpp/8 + z;		unsigned char lb = bitmap[LB];
				size_t RB = (m - x + (m-y)*width)*bpp/8 + z;	unsigned char rb = bitmap[RB];
				size_t RT = (m-y + x*width)*bpp/8 + z;			unsigned char rt = bitmap[RT];

				bitmap[LT] = rb;
				bitmap[LB] = rt;
				bitmap[RB] = lt;
				bitmap[RT] = lb;
			}
}

Image::Image(): 
	Res(),
	bpp(0),
	width(0),
	height(0),
	bitmap(NULL),
	bitmapSize(0),
	dimension(IMAGE_DIMENSION_NONE),
	format(IMAGE_FORMAT_NONE) 
{}

Image::~Image()
{
	unload(); 
}

std::string Image::getPath() const
{
    return path;
}
