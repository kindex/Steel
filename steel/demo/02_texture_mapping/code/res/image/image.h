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
public:
    typedef
        enum {none, texture1d, texture2d, cubemap, raw/*not registred bitmap*/}  imagerepresentationtype;
    typedef
        enum {none2, colormap, normalmap}  imagekindtype;

protected:
	public:// todo
    unsigned char* bitmap;
    int width, height, bpp;
    unsigned int id; // for OpenGL
    imagerepresentationtype representation;
    imagekindtype kind;

public:

    Image(): Res() { bpp = 0; id = 0;  bitmap = NULL; kind = none2; representation = none; }
    ~Image() { }

//    virtual void loadraw(const string& name)=0;
    void convertFromHeightMapToNormalMap();

    void makeCubeMap(const std::string& name);

//    void init(const string& name);

    void kill() {
    if (bitmap != NULL)
      free(bitmap);
      bitmap = NULL;
      kind = none2;
      representation = none;
      }

    unsigned int getid()   { return id;   }
    unsigned int getkind() { return kind; }

    //int Register1d();    int Register2d();

    bool init(int WIDTH, int HEIGHT, int BPP);

    void flipV(); // perevernut' Vertikalno |
    void flipH(); // perevernut' <->
    void putpixel(float x, float y, float r, float g, float b);
    void putpixelalfa(float x, float y, float r, float g, float b, float alfa);
    void putpixeladd(float x, float y, float r, float g, float b);
    void clear(float r, float g, float b);

 //   void operator=(const Image &A) { memcpy(this, &A, sizeof(Image)); } 
};


#endif
