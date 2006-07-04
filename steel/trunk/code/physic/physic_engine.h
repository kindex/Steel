/*id*********************************************************
    Unit: 3D PhysicEngine
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		����������� ���������� ������
		������������ �������� ��������.
		������������ ��������� ��������, �������� ���������� ��	��������.
************************************************************/


#ifndef PHYSIC_ENGINE_H
#define PHYSIC_ENGINE_H

#include "../res/conf/conf.h"

#include "physic_interface.h"

class PhysicEngine: public Engine
{
protected:
	// ����� [����������] ��������, ������� ���� ��������� � ������ ��� ��������� � ������� ��������� inject()
	steel::vector<PhysicObject*> objects;
	// ����������� �������� �� ��� �������
	v3 g;

	typedef std::map <uid, int> StorageHash;
	// ����������� ��������������� ������� �� ��������� � ������� storage
	StorageHash idHash;

public:
	struct TotalInfo
	{
		int collisionCount;
	} total;

public:
	virtual bool init(std::string _conf);
	virtual void deinit() { clear();}

	// ������� ��� ������� �� ������ (foreach remove)
	virtual bool clear();

	// ������������ �������� ���� ��������
	virtual bool process(steel::time globalTime, steel::time time) = 0; 

	// Inject ��������� ������ ��� ���������
	// ���� �������� ��������: uni, custom, none
	// � uni �� ����� ���� �����
	// � custom � none �� ����� ���� ����� � ����� uni
	// ��������� ������ � ��� ����� � ������ ��� ���������
	virtual bool inject(PhysicObject *object);
	virtual bool remove(PhysicObject *object);


	// ������ ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ������ �������
	virtual void makeStorageForObject(PhysicObject *object) = 0;
	virtual void deleteStorageForObject(int sid)  = 0;
	// ������ ����� ��� �������� �������������� ��������� (storage, ��� �������) - ��� ����� �������
	virtual void makeStorageForChildren(PhysicObject *object)  = 0;
	virtual void deleteStorageForChildren(int sid)  = 0;

	virtual void setGravitation(const v3 force) { g = force; }
};

#endif

