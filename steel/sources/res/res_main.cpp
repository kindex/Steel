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
#include "image/res_image.h"
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

template<class T, class T2>
T2* createClass(const std::string& filename, const std::string& base)
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

