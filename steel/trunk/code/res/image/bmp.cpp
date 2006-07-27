/*id*********************************************************
    Unit: Res-Image [Resources - Images - BMP (RGB bitmap)]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#include "bmp.h"
#include "../../common/logger.h"
#include "../rstream.h"
#include <string> 

Image* createBMP(const std::string filename)
{
	BMP *o = new BMP;
	if(o->init(filename))
	{
		o->setId(objectIdGenerator.genUid());
		return o;
	}
	else
	{
		delete o;
		return NULL;
	}
}

typedef unsigned char byte; // 1 byte
typedef unsigned short word; // 2 bytes
typedef unsigned long dword; // 4 bytes

typedef struct { // 14 bytes
	unsigned char	bfType[2];		//òèï ôàéëà (äëÿ áèòîâîãî îáðàçà - BM)
	dword	bfSize;		//ðàçìåð ôàéëà â dword
	word	bfReserved1;	//íå èñïîëüçóåòñÿ
	word	bfReserved2;	//íå èñïîëüçóåòñÿ
 	dword	bfOffbits;	//ñìåùåíèå äàííûõ áèòîâîãî îáðàçà îò
				//çàãîëîâêà â áàéòàõ
} bmpBITMAPFILEHEADER;

    typedef struct bmptagRGBQUAD {
	byte	rgbRed;		//èíòåíñèâíîñòü êðàñíîãî
	byte	rgbGreen;	//èíòåíñèâíîñòü çåëåíîãî
	byte	rgbBlue;	//èíòåíñèâíîñòü ãîëóáîãî
	byte	rgbRserved;	//íå èñïîëüçóåòñÿ
	} bmpRGBQUAD;


    typedef struct bmptagBITMAPINFOHEADER {
	dword	biSize;		//÷èñëî áàéò, çàíèìàåìûõ ñòðóêòóðîé BITMAPINFOHEADER - îáû÷íî èñïîëüçóåòñÿ äëÿ îáëåã÷åíèÿ äîñòóïà  ê òàáëèöå öâåòîâ.
 	dword	biWidth;	//øèðèíà áèòîâîãî îáðàçà â ïèêñåëàõ
 	dword	biHeight;	//âûñîòà áèòîâîãî îáðàçà â ïèêñåëàõ
	word	biPlanes;	//÷èñëî áèòîâûõ ïëîñêîñòåé óñòðîéñòâà - îïðåäåëÿåò ÷èñëî áèòîâûõ ïëîñêîñòåé;  îäíàêî,   ïîñêîëüêó öâåò êîäèðóåòñÿ ïîñëåäîâàòåëüíûìè áèòàìè, ýòî ÷èñëî âñåãäà ðàâíî 1.

	word	biBitCount;	//÷èñëî áèòîâ íà ïèêñåëü
        /*
ýòèì ïîëåì îïðåäåëÿåòñÿ ÷èñëî  öâåòîâ, èñïîëüçóå-
 ìûõ áèòîâûì îáðàçîì. Â çàâèñèìîñòè îò ñïîñîáà  êîäèðîâàíèÿ, ìîæåò
 ïðèíèìàòü çíà÷åíèÿ:
    1 - áèòîâûé îáðàç ìîíîõðîìíûé, è òàáëèöà öâåòîâ  äîëæíà ñîäåð-
 æàòü äâà ýëåìåíòà. Êàæäûé áèò â ìàññèâå äàííûõ êîäèðóåò îäèí ïèê-
 ñåë. Åñëè çíà÷åíèå áèòà - 0, òî ïèêñåë ñòàíîâèòñÿ  ïåðâûì  öâåòîì
 òàáëèöû; åñëè - 1, ïèêñåë ñòàíîâèòñÿ âòîðûì öâåòîì òàáëèöû.
    4 - áèòîâûé îáðàç èìååò ìàêñèìóì 16 öâåòîâ, è ìàññèâ bmiColors
 (òàáëèöà öâåòîâ) èìååò äî 16 ýëåìåíòîâ. Öâåò êàæäîãî  ïèêñåëà îï-
 ðåäåëÿåòñÿ ïî òàáëèöå öâåòîâ ïðè ïîìîùè  ÷åòûðåõáèòíîãî  èíäåêñà.
 Íàïðèìåð, åñëè ïåðâûé áàéò äàííûõ  èìååò  çíà÷åíèå  3Ah,  òî  ïðè
 îòîáðàæåíèè áèòîâîãî îáðàçà öâåò ïåðâîãî ïèêñåëà  îïðåäåëÿåò ÷åò-
 âåðòûé ýëåìåíò òàáëèöû öâåòîâ, à öâåò âòîðîãî - îäèííàäöàòûé.
    8 - áèòîâûé îáðàç èìååò ìàêñèìóì 256 öâåòîâ, è  òàáëèöà öâåòîâ
 èìååò äî 256 ýëåìåíòîâ. Êàæäûé  áàéò  ìàññèâà  äàííûõ  îïðåäåëÿåò
 öâåò îäíîãî ïèêñåëà.
    24 - áèòîâûé îáðàç èìååò ìàêñèìóì 2  â  24-é  ñòåïåíè  öâåòîâ.
 Òàáëèöà öâåòîâ ïóñòà, à öâåò ïèêñåëîâ  îïðåäåëÿåòñÿ  ïàêåòàìè  èç
 òðåõ áàéòîâ, îïèñûâàþùèìè öâåòîâûå èíòåíñèâíîñòè  êðàñíîãî, çåëå-
 íîãî è ãîëóáîãî öâåòîâ.  */
	dword	biCompression;	//òèï ñæàòèÿ
/*       òèï ñæàòèÿ. Ìîæåò ïðèíèìàòü çíà÷åíèÿ:
	BI_RGB - ñæàòèå îòñóòñòâóåò;
	BI_RLE8 - ñæàòèå äëÿ ôîðìàòà 8 áèò íà ïèêñåë;
	BI_RLE4 - ñæàòèå äëÿ ôîðìàòà 4 áèòà íà ïèêñåë.*/
	dword	biSizeImage;	//ðàçìåð êàðòèíêè â áàéòàõ
	dword	biXPelsPerMeter;//ãîðèçîíòàëüíîå ðàçðåøåíèå óñòðîéñòâà, ïèêñåë/ì
	dword	biYPelPerMeter;	//âåðòèêàëüíîå ðàçðåøåíèå óñòðîéñòâà, ïèêñåë/ì
	dword	biClrUsed;	//÷èñëî èñïîëüçóåìûõ öâåòîâ, ÷èñëî öâåòîâ, èñïîëüçóåìûõ äàííûè áèòîâûì îáðàçîì.  Åñëè 0, òî èñïîëüçóþòñÿ âñå öâåòà ïàëèòðû  (óêàçàííûå  â  ìàññèâå bmiColors).
	dword	biClrImportant; //÷èñëî "âàæíûõ" öâåòîâ
/* èñïîëüçóåòñÿ äëÿ óïðàâëåíèÿ  àëãîðèòìîì îòîá-
 ðàæåíèÿ öâåòîâ. Òàê, åñëè ÷åòûðå ðàçëè÷íûõ  ïðèëîæåíèÿ îòîáðàæàþò
 íà ýêðàíå ïî îäíîìó áèòîâîìó îáðàçó ñ 75 öâåòàìè êàæäûé, òî àäàï-
 òåð, âûâîäÿùèé 256 öâåòîâ îäíîâðåìåííî, íå  ñìîæåò  ïîëíîñòüþ àó-
 òåíòè÷íî îòîáðàçèòü íà ýêðàíå âñå 4 êàðòèíêè. Â  ýòîì  ñëó÷àå èñ-
 ïîëüçóåòñÿ ìåõàíèçì çàìåíû öâåòîâ - èùåòñÿ áèòîâûé  îáðàç  ñ íàè-
 ìåíüøèì ïðèîðèòåòîì è åãî "ëèøíèå" öâåòà çàìåíÿþòñÿ íàèáîëåå ïîä-
 õîäÿùèìè. */
    } bmpBITMAPINFOHEADER;

/*    Ïîñëå òîãî, êàê âñå ïàðàìåòðû áèòîâîãî  îáðàçà  îïðåäåëåíû,  â
 ôàéëå èäóò ñàìè  ñêàí-ñòðîêè  áèòîâîé  ïëîñêîñòè,  ïðè÷åì  ïåðâîé
 ñêàí-ñòðîêîé â ôîðìàòå DIB ñ÷èòàåòñÿ íèæíÿÿ ñêàí-ñòðîêà (ò.å. íà-
 ÷àëî êîîðäèíàò íàõîäèòñÿ â ëåâîì íèæíåì óãëó èçîáðàæåíèÿ).

    Ñêàí-ñòðîêè âûðîâíåíû ïî 32-áèòíîé ãðàíèöå - dword !!!*/

    typedef struct bmptagBITMAPINFO {
    bmpBITMAPINFOHEADER	 bmiHeader;
    bmpRGBQUAD		 bmiColors[1];
    } bmpBITMAPINFO;


bool BMP::init(const std::string name)
{  
	// TODO: load through CLASS (WADfile, disk,...)
	bmpBITMAPFILEHEADER fh; // file header
	bmpBITMAPINFOHEADER ih;

/*	if(sizeof(fh)!= 14) 
	{	
		steel::log.out("Res/Image/BMP: head struct size 14 != %d", sizeof(fh));
		throw;
	}
*/

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

	f.read((char*)&ih.biSize, sizeof(ih.biSize));

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
		kind	= IMAGE_2D;
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

