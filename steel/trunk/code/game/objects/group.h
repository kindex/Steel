/*id*********************************************************
    Unit: game/objects/group
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        ������ ��� Division
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

#include "../game_obj.h"

class GameGroup: public GameObjSet
{
public:
	std::string conf;

	bool init(ScriptLine	&s,		ResCollection &res);
	bool load(ResCollection *res,	GameObj *global);
};

#endif
