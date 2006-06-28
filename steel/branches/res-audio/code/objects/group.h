/*id*********************************************************
    Unit: objects/group
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		��������� ��� ������ ��������.
		� ���� ������������� ������ ������� � �������� ��� ������ �����.
		���� �� ������-������ �� ����� �������� ����, ������� ���
		���� �����, �� ������ ���������� �� ������ � ��������� ������ ������.

		��� ����� (�������������� ������ ������ ���������, � �� ����������� �
		������� ��������.
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
