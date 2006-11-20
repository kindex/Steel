/*id*********************************************************
	File: engine/id_generator.h
	Unit: engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		���������� ������������� �������� ������� ��� �������
 ************************************************************/

#ifndef ENGINE__ID_GENERATOR_H
#define ENGINE__ID_GENERATOR_H

typedef unsigned int uid;

class IdGenerator
{
protected:
	uid freeId;
public:
	IdGenerator(void) { freeId = 1; }
	uid genUid(void);
};

extern IdGenerator objectIdGenerator;

#endif
