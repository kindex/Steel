/*id*********************************************************
	File: res/image/tga.cpp
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Класс для загрузки BMP-файла в класс Image
 ************************************************************/

#include "../../steel.h"
#include "tga.h"
#include "../../common/logger.h"
#include "../rstream.h"
#include <string> 

Image* createTGA(const std::string& filename) 
{
	TGA *o = new TGA;
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

bool TGA::init(const std::string& name)
{  
	rstream f;
    path = name;

	if (!f.open(name, "tga"))
	{
//		log_msg("warning res image tga", string("Res/Image/TGA: cannot open file ")+ name.c_str());
		return false;
	}

	unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	unsigned char TGAcompare[12], header[6], temp;						// Used To Compare TGA Header

	f.read(TGAcompare, sizeof(TGAcompare));
		
	if (memcmp(TGAheader, TGAcompare, sizeof(TGAheader))!=0)
    {
		return false;	// Does The Header Match What We Want?
    }

	f.read(header, 6);// If So Read Next 6 Header Bytes

	if (!createImage(header[1] * 256 + header[0], header[3] * 256 + header[2], header[4])) return false;

	f.read(bitmap, bitmapSize);	// Does The Image Size Match The Memory Reserved?

	if (bpp==32) format = IMAGE_RGBA;
	if (bpp==24) format = IMAGE_RGB;
	dimension = IMAGE_2D;

	int ByPP = bpp/8;

	for (size_t i=0; i < bitmapSize; i += ByPP)	// Loop Through The Image Data
	{											// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = bitmap[i];						// Temporarily Store The Value At Image Data 'i'
		bitmap[i] = bitmap[i + 2];				// Set The 1st Byte To The Value Of The 3rd Byte
		bitmap[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	return true;
}
