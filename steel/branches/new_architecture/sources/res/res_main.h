/*id*******************************************************************************
	File: res/res_main.h
	Unit: res
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Создание коллекций ресурсов для каждого типа ресурса
		и регистрации всех типов ресурсов
**************************************************************************************/

#ifndef __RES_MAIN_H
#define __RES_MAIN_H

#include "../steel.h"
#include "res.h"

#include "config/config_collection.h"
#include "image/image.h"
#include "model/model.h"
#include "material/material.h"
#include "text/text.h"
#include "audio/audio.h"

// глобальные коллекции ресурсов
extern ResCollectionConfig		resConfig;
extern ResCollection<Image>		resImage;
extern ResCollection<Model>		resModel;
extern ResCollection<Material>	resMaterial;
extern ResCollection<Text>		resText;
extern ResCollection<Audio>		resAudio;


bool registerResources(void);

#endif
