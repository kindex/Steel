// Файл для регистрации всех типов ресурсов

#ifndef __RES_MAIN_H
#define __RES_MAIN_H

#include "res.h"

#include "conf/conf.h"
#include "image/image.h"
#include "model/model.h"
#include "material/material.h"
#include "script/script.h"

// глобальная коллекция ресурсов
extern ResCollection<Config> resConfig;
extern ResCollection<Image> resImage;
extern ResCollection<Model> resModel;
extern ResCollection<Material> resMaterial;
extern ResCollection<Script> resScript;


bool registerResources();

#endif
