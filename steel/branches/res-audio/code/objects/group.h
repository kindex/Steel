/*id*********************************************************
    Unit: objects/group
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Контейнер для других объектов.
		К нему прикрепляются другие объекты и движутся как единое целое.
		Если на какого-нибудь из детей дествует сила, большая чем
		сила связи, то объект отделяется от группы и передаётся предку группы.

		Все имена (идентификаторы внутри группы уникальны, и не конфлинтуют с
		другими группами.
 ************************************************************/

#ifndef __GAME_GROUP_H
#define __GAME_GROUP_H

#include "game_obj.h"

class GameGroup: public GameObjSet
{
public:
	std::string conf;

	bool init(ScriptLine	&s);
	bool load(GameObj *global);
};

#endif
