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
		�������� ��������� �������� ��� ������� ���� �������
		� ����������� ���� ����� ��������
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

// ���������� ��������� ��������
extern ResCollectionConfig		resConfig;
extern ResCollection<Image>		resImage;
extern ResCollection<Model>		resModel;
extern ResCollection<Material>	resMaterial;
extern ResCollection<Text>		resText;
extern ResCollection<Audio>		resAudio;


bool registerResources(void);

#endif
