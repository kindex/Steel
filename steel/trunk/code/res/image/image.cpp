/*id*********************************************************
    Unit: Res-Image [Resources - Images (RGB bitmap)]
    Part of: Steel engine Res unit
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс для хранения изображения в виде RGB (без загрузки)
 ************************************************************/

#include "image.h"
#include "../../math/geometry.h"
#include "../../math/maths.h"

bool Image::init(int WIDTH, int HEIGHT, int BPP)
{
    width = WIDTH;
    height = HEIGHT;
    bpp = BPP;
    bitmap = (unsigned char*)malloc(bpp/8*width*height);
	bitmapSize = bpp/8*width*height;
    return bitmap != NULL;
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

  for (int y = 0; y<height; y++)
    for (int x = 0; x<(width-1)/2; x++)
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
    int x = (int)(X*width);
    if (x<0 || x>= width) return;

    int y = (int)(Y*height);
    if (y<0 || y>= height) return;

    int bpl = width*3;
    bitmap[y*bpl + x*3 + 0] = (int)(r*255);
    bitmap[y*bpl + x*3 + 1] = (int)(g*255);
    bitmap[y*bpl + x*3 + 2] = (int)(b*255);
}

void Image::putpixeladd(float X, float Y, float r, float g, float b)
{
    if (bitmap == NULL) return ;
    int x = (int)(X*width);
    if (x<0 || x>= width) return;

    int y = (int)(Y*height);
    if (y<0 || y>= height) return;

    int bpl = width*3;
    int a;
    a = (int)bitmap[y*bpl + x*3 + 0] + (int)(r*255); if (a<0) a = 0; if (a>255) a = 255; bitmap[y*bpl + x*3 + 0] = a;
    a = (int)bitmap[y*bpl + x*3 + 1] + (int)(g*255); if (a<0) a = 0; if (a>255) a = 255; bitmap[y*bpl + x*3 + 1] = a;
    a = (int)bitmap[y*bpl + x*3 + 2] + (int)(b*255); if (a<0) a = 0; if (a>255) a = 255; bitmap[y*bpl + x*3 + 2] = a;
}


void Image::putpixelalfa(float X, float Y, float r, float g, float b, float alfa)
{
    if (bitmap == NULL) return ;
    int x = (int)(X*width);
    if (x<0 || x>= width) return;

    int y = (int)(Y*height);
    if (y<0 || y>= height) return;

    int bpl = width*3;
    bitmap[y*bpl + x*3 + 0] = (unsigned char)(bitmap[y*bpl + x*3 + 0]*(1-alfa) + alfa*r*255);
    bitmap[y*bpl + x*3 + 1] = (unsigned char)(bitmap[y*bpl + x*3 + 1]*(1-alfa) + alfa*g*255);
    bitmap[y*bpl + x*3 + 2] = (unsigned char)(bitmap[y*bpl + x*3 + 2]*(1-alfa) + alfa*b*255);
}


void Image::clear(float r, float g, float b)
{
    if (bitmap == NULL) return;
    int bpl = width*3;
    for(int y=0; y<height; y++)
        for(int x=0; x<width; x++)
        {
            bitmap[y*bpl + x*3 + 0] = (int)(r*255);
            bitmap[y*bpl + x*3 + 1] = (int)(g*255);
            bitmap[y*bpl + x*3 + 2] = (int)(b*255);
        }
}

void Image::convertFromHeightMapToNormalMap()
{
    int bpl = width*3;
    unsigned char *a = (unsigned char*)malloc(bpl); // RGB
    unsigned char *b = (unsigned char*)malloc(bpl); // RGB

    memcpy(b, &bitmap[(height-1)*bpl], bpl);

    for(int y=0; y<height; y++)
    {
        memcpy(a, b, bpl);
        memcpy(b, &bitmap[y*bpl], bpl);

        for(int x=0; x<width; x++)
        {
            unsigned char A = a[x*3];
//            unsigned char B = b[x*3];
            unsigned char C = b[(x+width-1)%width*3];
            unsigned char D = b[(x+1)%width*3];
            unsigned char E = bitmap[bpl*(y+1)%height+x*3];

            float X, Y, Z;

            X = ((float)C-(float)D)/255.0f;
            Y = ((float)E-A)/255.0f;

            //Z = 1 - sqr(X) -sqr(Y);
			Z=1-X*X-Y*Y;

            float r = (X+1)/2;
            float g = (Y+1)/2;
            float b = (Z+1)/2;

            bitmap[bpl*y + x*3 + 0] = (unsigned char)(r*255);
            bitmap[bpl*y + x*3 + 1] = (unsigned char)(g*255);
            bitmap[bpl*y + x*3 + 2] = (unsigned char)(b*255);
        }
    }
    free(a);
    free(b);
}
