#ifndef __GAME_GROUP_H
#define __GAME_GROUP_H

#include "../game_obj.h"

/*
��������� ��� ������ ��������.
� ���� ������������� ������ ������� � �������� ��� ������ �����.
���� �� ������-������ �� ����� �������� ����, ������� ���
���� �����, �� ������ ���������� �� ������ � ��������� ������ ������.

��� ����� (�������������� ������ ������ ���������, � �� ����������� �
������� ��������.
*/

class GameGroup: public GameObjSet
{
public:
	std::string conf;

	bool init(ScriptLine	&s,		ResCollection &res);
	bool load(ResCollection *res,	GameObj *global);
};

#endif
