/*id*******************************************************************************
	File: res/res_main.cpp
	Unit: res
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Создание коллекций ресурсов для каждого типа ресурса
		и регистрации всех типов ресурсов
**************************************************************************************/

#include "../steel.h"
#include "res_main.h"
#include "image/bmp.h"
#include "image/tga.h"
#include "model/3ds.h"
#include "config/config_collection.h"
#include "text/text_file.h"
#include "audio/wav.h"
#include "audio/ogg.h"


ResCollectionConfig		resConfig;
ResCollection<Image>	resImage;
ResCollection<Model>	resModel;
ResCollection<Text>		resText;
ResCollection<Audio>	resAudio;


// Image filename:
// name[.cube][.normal|.height|.alpha][.bmp|.tga]

Image* createBitmap(const std::string filename)
{
	Image *r;
	// try to load image2d
	if((r = createBMP(filename))!= NULL) return r;
	if((r = createTGA(filename))!= NULL) return r;
	return NULL;
}

Image* createImageFormat(const std::string filename)
{
	Image* r;
	// try to load image2d
	if ((r = createBitmap(filename)) != NULL)
	{
		Image* alpha;
		if ((alpha = createBitmap(filename + ".alpha")) != NULL)
		{
			r->addAlphaChannel(alpha);
			delete alpha;
		}

		return r;
	}
	if((r = createBitmap(filename + ".normal"))	!= NULL)
	{
		((Image*)r)->setFormat(IMAGE_RGB);
		return r;
	}
	if((r = createBitmap(filename + ".height")) != NULL)
	{
		((Image*)r)->convertFromHeightMapToNormalMap();
		((Image*)r)->setFormat(IMAGE_RGB);
		return r;
	}

	return NULL;
}


Image* createImage(const std::string filename, const std::string base)
{
	Image *r;
	// try to load image2d
	if((r = createImageFormat(base + "/" +  filename)) != NULL) return r;
	// try to load cubemap
	if((r = createImageFormat(base + "/" + filename + ".cube")) != NULL)
	{
		((Image*)r)->setDimension(IMAGE_CUBE);
		return r;
	}
	return NULL;
}



template<class T, class T2>
T2* createClass(const std::string filename, const std::string base)
{
	if(filename.empty()) return false;

	T* object = new T;

	if(object->init(filename, base))
	{
		object->setId(objectIdGenerator.genUid());
		return object;
	}
	else
	{
        delete object;
		return NULL;
    }
}

bool registerResources()
{
	resConfig.setId("config");

	resImage.setId("image");	resImage.registerResLoader(createImage);
	resModel.setId("model");	resModel.registerResLoader(createClass<Model_3DS, Model>);
	resText.setId("text");	resText.registerResLoader(createClass<TextFile, Text>);
	resAudio.registerResLoader(createClass<WAV, Audio>);            resAudio.setId("audio");
	resAudio.registerResLoader(createClass<OGG, Audio>);            resAudio.setId("audio");

	return true;
}

