/*id*********************************************************
    Unit: Res-Image [Resources - Images - BMP (RGB bitmap)]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#include "tga.h"
#include "../../common/logger.h"
#include <string> 

using namespace std;

Res* createTGA(const std::string filename, ResCollection *res) 
{
	TGA *o = new TGA;
	if(o->init(filename)) 
	{
		o->setId(res->genUid());
		return o;
	}
	else
		return NULL;
}

bool TGA::init(const std::string name)
{  
	string file = "../res/image/" + name + ".tga";
	rstream f(file);

	if(!f.good())
	{
		alog.msg("warning res image tga", string("Res/Image/TGA: cannot open file ")+ file.c_str());
		return false;
	}

	unsigned char		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	unsigned char		TGAcompare[12], header[6], temp;						// Used To Compare TGA Header

	f.read(TGAcompare, sizeof(TGAcompare));
		
	if(memcmp(TGAheader, TGAcompare, sizeof(TGAheader))!=0)
		return false;	// Does The Header Match What We Want?

	f.read(header, 6);// If So Read Next 6 Header Bytes

	if(!Image::init(header[1] * 256 + header[0], header[3] * 256 + header[2], header[4])) return false;

	f.read(bitmap, bitmapSize);	// Does The Image Size Match The Memory Reserved?
	int ByPP = bpp/8;
	for(int i=0; i<bitmapSize; i += ByPP)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = bitmap[i];								// Temporarily Store The Value At Image Data 'i'
		bitmap[i] = bitmap[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		bitmap[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	return true;
}
