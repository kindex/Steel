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
	unsigned char	bfType[2];		//��� ����� (��� �������� ������ - BM)
	dword	bfSize;		//������ ����� � dword
	word	bfReserved1;	//�� ������������
	word	bfReserved2;	//�� ������������
 	dword	bfOffbits;	//�������� ������ �������� ������ ��
				//��������� � ������
} bmpBITMAPFILEHEADER;

    typedef struct bmptagRGBQUAD {
	byte	rgbRed;		//������������� ��������
	byte	rgbGreen;	//������������� ��������
	byte	rgbBlue;	//������������� ��������
	byte	rgbRserved;	//�� ������������
	} bmpRGBQUAD;


    typedef struct bmptagBITMAPINFOHEADER {
	dword	biSize;		//����� ����, ���������� ���������� BITMAPINFOHEADER - ������ ������������ ��� ���������� �������  � ������� ������.
 	dword	biWidth;	//������ �������� ������ � ��������
 	dword	biHeight;	//������ �������� ������ � ��������
	word	biPlanes;	//����� ������� ���������� ���������� - ���������� ����� ������� ����������;  ������,   ��������� ���� ���������� ����������������� ������, ��� ����� ������ ����� 1.

	word	biBitCount;	//����� ����� �� �������
        /*
���� ����� ������������ �����  ������, ���������-
 ��� ������� �������. � ����������� �� �������  �����������, �����
 ��������� ��������:
    1 - ������� ����� �����������, � ������� ������  ������ �����-
 ���� ��� ��������. ������ ��� � ������� ������ �������� ���� ���-
 ���. ���� �������� ���� - 0, �� ������ ����������  ������  ������
 �������; ���� - 1, ������ ���������� ������ ������ �������.
    4 - ������� ����� ����� �������� 16 ������, � ������ bmiColors
 (������� ������) ����� �� 16 ���������. ���� �������  ������� ��-
 ���������� �� ������� ������ ��� ������  ��������������  �������.
 ��������, ���� ������ ���� ������  �����  ��������  3Ah,  ��  ���
 ����������� �������� ������ ���� ������� �������  ���������� ���-
 ������ ������� ������� ������, � ���� ������� - ������������.
    8 - ������� ����� ����� �������� 256 ������, �  ������� ������
 ����� �� 256 ���������. ������  ����  �������  ������  ����������
 ���� ������ �������.
    24 - ������� ����� ����� �������� 2  �  24-�  �������  ������.
 ������� ������ �����, � ���� ��������  ������������  ��������  ��
 ���� ������, ������������ �������� �������������  ��������, ����-
 ���� � �������� ������.  */
	dword	biCompression;	//��� ������
/*       ��� ������. ����� ��������� ��������:
	BI_RGB - ������ �����������;
	BI_RLE8 - ������ ��� ������� 8 ��� �� ������;
	BI_RLE4 - ������ ��� ������� 4 ���� �� ������.*/
	dword	biSizeImage;	//������ �������� � ������
	dword	biXPelsPerMeter;//�������������� ���������� ����������, ������/�
	dword	biYPelPerMeter;	//������������ ���������� ����������, ������/�
	dword	biClrUsed;	//����� ������������ ������, ����� ������, ������������ ������ ������� �������.  ���� 0, �� ������������ ��� ����� �������  (���������  �  ������� bmiColors).
	dword	biClrImportant; //����� "������" ������
/* ������������ ��� ����������  ���������� ����-
 ������� ������. ���, ���� ������ ���������  ���������� ����������
 �� ������ �� ������ �������� ������ � 75 ������� ������, �� ����-
 ���, ��������� 256 ������ ������������, ��  ������  ��������� ��-
 �������� ���������� �� ������ ��� 4 ��������. �  ����  ������ ��-
 ���������� �������� ������ ������ - ������ �������  �����  � ���-
 ������� ����������� � ��� "������" ����� ���������� �������� ���-
 ��������. */
    } bmpBITMAPINFOHEADER;

/*    ����� ����, ��� ��� ��������� ��������  ������  ����������,  �
 ����� ���� ����  ����-������  �������  ���������,  ������  ������
 ����-������� � ������� DIB ��������� ������ ����-������ (�.�. ��-
 ���� ��������� ��������� � ����� ������ ���� �����������).

    ����-������ ��������� �� 32-������ ������� - dword !!!*/

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
