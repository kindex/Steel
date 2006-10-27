/*id*********************************************************
	File: objects/audio_ambient.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		����� ������ - ���������� ����.
		���������� ��� ���� - ��� �������� �� ����� ���� � 
		���������. ��� ���� ����� ���  ������ � Quake.
		����� � ��������� - ��� ����������� ��������� �����.
 ************************************************************/

#ifndef __AUDIO_AMBIENT_H
#define __AUDIO_AMBIENT_H

#include "audio_object.h"

class AudioAmbient : public AudioSourceRes
{
public:
	AudioAmbient(void); //:sound(NULL), gain(1.0f), pitch(1.0f), isLooped(false) {}
};

#endif
