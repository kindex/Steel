/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		���� ���� �������� ����������� � steel engine � ������
		�������� ������������� ������. � ���� ����� �������� ������� ����
		��� ����������� �����.
 ************************************************************/

#ifndef INTRO_H
#define INTRO_H

#include <steel.h>
#include <res/res.h>

#include <graph/graph_engine.h>
#include <audio/audio_engine.h>
#include <game/game.h>


/*
�����, �������� ������� ����
*/
class Demo: public Steel
{
public:
	bool init(Config *_conf, Input *_input);
	virtual GameObjectFactory* createGameObjectFactory() const;
};

#endif
