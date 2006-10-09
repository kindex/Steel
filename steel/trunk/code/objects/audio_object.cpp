/*id*********************************************************
	File: objects/audio_object.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		����� ������, ������� ����� �� ��������
 ************************************************************/

#include "audio_object.h"

AudioSourceRes::AudioSourceRes(void)
{
	sound = NULL;
	gain = 1.0f;
	pitch = 1.0f;
	isLoop = false;
}
