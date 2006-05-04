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

class GameGroup: public GameObjDummy
{
public:
	bool init(ScriptLine	&s,		ResCollection &res);
	bool load(std::string script,	ResCollection *res);
};

#endif
