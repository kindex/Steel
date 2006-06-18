/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Bond [Andrey Bondarenko]
    License:
        Steel Engine License
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
#include "res/res.h"
#include "res/conf/conf.h"
#include "math/vector3d.h"
#include "interface.h"
#include "helper.h"

#include <string>

class Engine: public steelAbstract
{
protected:
	EngineHelper	*helper;
public:
	Config			*conf;

	virtual ~Engine() {}
	virtual bool init(std::string _conf) = 0; // initialization

	virtual bool clear() = 0; 
//	protottype: virtual bool inject(Interface *object) { return false; }; // add object to process by engine
//	virtual	bool remove(Interface *object);
//	virtual bool process() = 0; // add object to process by engine
	virtual bool deinit() = 0;
	virtual void bindHelper(EngineHelper *_helper) { helper = _helper; } 
	// TODO: ������ ���. �� ��������� � ���� ������
};

#endif
