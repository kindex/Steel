/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
        Steel Engine License
    Description:
		������� test() ����� ��� ������������� ������������ ����. 
		Ÿ ����� ������ ��� ������, �� ������ ��������� ���������.
		���� test ��������� true, �� ������ ���������� ��������, �����
		��������� ��������� ����������.

		��� ������� ������ ������������ �������� debug()
 ************************************************************/

#include "_cpp.h"
#include "common/logger.h"
#include "res/res_main.h"

bool test()
{
	Text *t = resText.add("a.txt");

	return true;
}
