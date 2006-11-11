/*id*********************************************************
	File: objects/scene.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Загрузка уровня из *.scene файла.
 ************************************************************/
#include "../steel.h"
#ifndef __OBJECTS__SCENE_H
#define __OBJECTS__SCENE_H
#include "game_obj.h"

GameObject	*createGameObject(Config *conf);

class Scene: public GameObject
{
public:
	virtual bool InitFromConfig(Config *conf);
protected:
	svector<GameObject*> objects;

};

#endif
