/*id*********************************************************
	File: res/image/bmp/bmp_load.cpp
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#include "../../../steel.h"
#include "../bmp.h"
#include "../../../common/logger.h"
#include "../../rstream.h"
#include <string> 

typedef unsigned char byte; // 1 byte
typedef unsigned short word; // 2 bytes
typedef unsigned long dword; // 4 bytes

typedef struct { // 14 bytes
	unsigned char	bfType[2];	//2	
	dword	bfSize;				//4
	word	bfReserved1;		//2
	word	bfReserved2;		//2
 	dword	bfOffbits;			//4

} bmpBITMAPFILEHEADER;

    typedef struct bmptagRGBQUAD {
	byte	rgbRed;		
	byte	rgbGreen;	
	byte	rgbBlue;	
	byte	rgbRserved;	
	} bmpRGBQUAD;


    typedef struct bmptagBITMAPINFOHEADER {
	dword	biSize;		
 	dword	biWidth;	
 	dword	biHeight;	
	word	biPlanes;	

	word	biBitCount;	
	dword	biCompression;	
	dword	biSizeImage;	
	dword	biXPelsPerMeter;
	dword	biYPelPerMeter;	
	dword	biClrUsed;	
	dword	biClrImportant; 
    } bmpBITMAPINFOHEADER;

    typedef struct bmptagBITMAPINFO {
    bmpBITMAPINFOHEADER	 bmiHeader;
    bmpRGBQUAD		 bmiColors[1];
    } bmpBITMAPINFO;


bool BMP::init(const std::string name)
{  
	// TODO: load through CLASS (WADfile, disk,...)
	bmpBITMAPFILEHEADER fh; // file header
	bmpBITMAPINFOHEADER ih;

//	cassert(sizeof(fh)== 14);

//	int tty2 = sizeof(ih);
	std::string file = name;
	rstream f;

	if(!f.open(file, "bmp")) 
	{
// 		steel::log.out("Res/Image/BMP: cannot open file %s", file.c_str());
		return false;
	}

	if(!f.good()) return false;

	f.read(&fh.bfType[0], 2);
    f.read(&fh.bfSize, 4);
	f.read(&fh.bfReserved1, 2);
	f.read(&fh.bfReserved2, 2);
    f.read(&fh.bfOffbits, 4);
	f.read(&ih.biSize, 4);

	int siz = ih.biSize-sizeof(ih.biSize);

	f.read((char*)&ih.biWidth, siz);

	if (ih.biSizeImage == 0)
		ih.biSizeImage = ih.biHeight * ih.biWidth * (ih.biBitCount /8);

	unsigned int bpl  = ih.biWidth*(ih.biBitCount/8); // Bypes per line
///unsigned int rbpl = ih.biSizeImage / ih.biHeight; // real bpl

int r;

//    image.init(ih.biWidth, ih.biHeight, 24);
	switch (ih.biBitCount)
	{
	case 24:
  //   fseek(file,fh.bfOffbits, SEEK_SET );
		bitmapSize = ih.biSizeImage;
		if(!createImage(ih.biWidth, ih.biHeight, ih.biBitCount)) return false;

/*     for (unsigned int i=0; i<ih.biHeight; i++)
     {
        if ( (r=fread(bitmap+i*bpl, bpl, 1, file)) != 1) throw;
        if (rbpl>bpl)
          fseek(file,rbpl-bpl, SEEK_CUR );
     }*/
		f.read(this->bitmap, ih.biSizeImage);
     // SWAP BGR ->  RGB
		for (unsigned int i=0; i<ih.biHeight; i++)
		{
        for (unsigned int j=0; j<ih.biWidth; j++) // TODO - change format when registring
        {
          r = bitmap[i*bpl + j*3];
          bitmap[i*bpl + j*3] = bitmap[i*bpl + j*3 + 2];
          bitmap[i*bpl + j*3 + 2] = r ;
        }
     }
		dimension	= IMAGE_2D;
		format	= IMAGE_RGB;

     break;
//   case 8:
//     colcnt = 256;
//     palsize = colcnt*4;/*sizeof(bmpRGBQUAD)*/
/*     pal = (bmpRGBQUAD*)malloc(palsize);
     if (fread(pal, palsize, 1, file) != 1) throw;
     fseek(file,fh.bfOffbits, SEEK_SET );
     line = (char*)malloc(rbpl);

     image.bitmap = (char*)malloc(ih.biWidth*ih.biHeight*3);
     for (unsigned int i=0; i<ih.biHeight; i++)
     {
        if (fread(line, rbpl, 1, file) != 1) throw;

        for(unsigned int j=0; j<image.width; j++)
        {
           bitmap[i*bpl + j*3 + 0] = pal[line[j]].rgbBlue;
           bitmap[i*bpl + j*3 + 1] = pal[line[j]].rgbGreen;
           bitmap[i*bpl + j*3 + 2] = pal[line[j]].rgbRed;
        }
     }

     free(line);
     free(pal);
     break;*/

	   default: return false; // unsupported format 
   }
 
   return true;
}

