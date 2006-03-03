//---------------------------------------------------------------------------


#pragma hdrstop

#include "bmp.h"

#include <stdio.h> // TODO - load file with res.h


//#pragma option -a1 // align on bytes !!!!!!!!!! 4tobi pravilno zagruzit' headeri, 
// 4tobi nebilo viravnivanija

typedef unsigned char byte; // 1 byte
typedef unsigned short word; // 2 bytes
typedef unsigned long dword; // 4 bytes

typedef struct { // 14 bytes
	unsigned char	bfType[2];		//тип файла (для битового образа - BM)
	dword	bfSize;		//размер файла в dword
	word	bfReserved1;	//не используется
	word	bfReserved2;	//не используется
 	dword	bfOffbits;	//смещение данных битового образа от
				//заголовка в байтах
} bmpBITMAPFILEHEADER;

    typedef struct bmptagRGBQUAD {
	byte	rgbRed;		//интенсивность красного
	byte	rgbGreen;	//интенсивность зеленого
	byte	rgbBlue;	//интенсивность голубого
	byte	rgbRserved;	//не используется
	} bmpRGBQUAD;


    typedef struct bmptagBITMAPINFOHEADER {
	dword	biSize;		//число байт, занимаемых структурой BITMAPINFOHEADER - обычно используется для облегчения доступа  к таблице цветов.
 	dword	biWidth;	//ширина битового образа в пикселах
 	dword	biHeight;	//высота битового образа в пикселах
	word	biPlanes;	//число битовых плоскостей устройства - определяет число битовых плоскостей;  однако,   поскольку цвет кодируется последовательными битами, это число всегда равно 1.

	word	biBitCount;	//число битов на пиксель
        /*
этим полем определяется число  цветов, используе-
 мых битовым образом. В зависимости от способа  кодирования, может
 принимать значения:
    1 - битовый образ монохромный, и таблица цветов  должна содер-
 жать два элемента. Каждый бит в массиве данных кодирует один пик-
 сел. Если значение бита - 0, то пиксел становится  первым  цветом
 таблицы; если - 1, пиксел становится вторым цветом таблицы.
    4 - битовый образ имеет максимум 16 цветов, и массив bmiColors
 (таблица цветов) имеет до 16 элементов. Цвет каждого  пиксела оп-
 ределяется по таблице цветов при помощи  четырехбитного  индекса.
 Например, если первый байт данных  имеет  значение  3Ah,  то  при
 отображении битового образа цвет первого пиксела  определяет чет-
 вертый элемент таблицы цветов, а цвет второго - одиннадцатый.
    8 - битовый образ имеет максимум 256 цветов, и  таблица цветов
 имеет до 256 элементов. Каждый  байт  массива  данных  определяет
 цвет одного пиксела.
    24 - битовый образ имеет максимум 2  в  24-й  степени  цветов.
 Таблица цветов пуста, а цвет пикселов  определяется  пакетами  из
 трех байтов, описывающими цветовые интенсивности  красного, зеле-
 ного и голубого цветов.  */
	dword	biCompression;	//тип сжатия
/*       тип сжатия. Может принимать значения:
	BI_RGB - сжатие отсутствует;
	BI_RLE8 - сжатие для формата 8 бит на пиксел;
	BI_RLE4 - сжатие для формата 4 бита на пиксел.*/
	dword	biSizeImage;	//размер картинки в байтах
	dword	biXPelsPerMeter;//горизонтальное разрешение устройства, пиксел/м
	dword	biYPelPerMeter;	//вертикальное разрешение устройства, пиксел/м
	dword	biClrUsed;	//число используемых цветов, число цветов, используемых данныи битовым образом.  Если 0, то используются все цвета палитры  (указанные  в  массиве bmiColors).
	dword	biClrImportant; //число "важных" цветов
/* используется для управления  алгоритмом отоб-
 ражения цветов. Так, если четыре различных  приложения отображают
 на экране по одному битовому образу с 75 цветами каждый, то адап-
 тер, выводящий 256 цветов одновременно, не  сможет  полностью ау-
 тентично отобразить на экране все 4 картинки. В  этом  случае ис-
 пользуется механизм замены цветов - ищется битовый  образ  с наи-
 меньшим приоритетом и его "лишние" цвета заменяются наиболее под-
 ходящими. */
    } bmpBITMAPINFOHEADER;

/*    После того, как все параметры битового  образа  определены,  в
 файле идут сами  скан-строки  битовой  плоскости,  причем  первой
 скан-строкой в формате DIB считается нижняя скан-строка (т.е. на-
 чало координат находится в левом нижнем углу изображения).

    Скан-строки выровнены по 32-битной границе - dword !!!*/

    typedef struct bmptagBITMAPINFO {
    bmpBITMAPINFOHEADER	 bmiHeader;
    bmpRGBQUAD		 bmiColors[1];
    } bmpBITMAPINFO;



bool BMP::init(std::string& name)
{   // TODO: load through CLASS (WADfile, disk,...)
	bmpBITMAPFILEHEADER fh; // file header
	bmpBITMAPINFOHEADER ih;

	if(sizeof(fh)!= 14) throw;
//	int tty2 = sizeof(ih);

	rstream f(name);

	f.read((char*)&fh,sizeof(fh));

	f.read((char*)&ih.biSize, sizeof(ih.biSize));

	int siz = ih.biSize-sizeof(ih.biSize);

	f.read((char*)&ih.biWidth,siz);

	if (ih.biSizeImage == 0)
		ih.biSizeImage = ih.biHeight * ih.biWidth * (ih.biBitCount /8);

	unsigned int bpl  = ih.biWidth*(ih.biBitCount/8); // Bypes per line
///unsigned int rbpl = ih.biSizeImage / ih.biHeight; // real bpl

int r;

//    image.init(ih.biWidth, ih.biHeight, 24);
	width = ih.biWidth;
	height = ih.biHeight;
	bpp = 24;
	switch (ih.biBitCount)
	{
	case 24:
  //   fseek(file,fh.bfOffbits, SEEK_SET );
		bitmap = (unsigned char*)malloc(ih.biSizeImage);

/*     for (unsigned int i=0; i<ih.biHeight; i++)
     {
        if ( (r=fread(bitmap+i*bpl, bpl, 1, file)) != 1) throw;
        if (rbpl>bpl)
          fseek(file,rbpl-bpl, SEEK_CUR );
     }*/
		f.read((char*)bitmap, ih.biSizeImage);
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
   kind = Image::colormap;
   return true;
}

bool BMP::unload()
{
	return false;
}


//#pragma option -a.      // restore command-line option



//#pragma package(smart_init)
