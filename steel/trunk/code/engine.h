/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
		* Bond [Andrey Bondarenko]
    Licence:
        ������ ��� Division
    Description:
		����� Interface - �������� ������, �� �������� ������ ������������� 
		������, ������� ������ �������������� ������� Engine.
		����� Engine (������) ����� ������� inject() �������� �������, �������
		������ ���� ����������, �� ������������ �� ������ ������ process().
		��� ����� ������� ������ ������������ ��������� (interface) ������ 
		����������� � �������.
 ************************************************************/

#ifndef ENGINE_H
#define ENGINE_H

#include "steel.h"

/*
protottype:
*/
class Interface{};


class Engine: public steelAbstract
{
public:
	virtual ~Engine() {}
	virtual bool init() = 0; // initialization
//	protottype: virtual bool inject(Interface *object) { return false; }; // add object to process by engine
	virtual bool clear() = 0; 
	virtual bool process() = 0; // add object to process by engine
	virtual bool deinit() = 0;
};

#endif
