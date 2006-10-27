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

#include "audio_ambient.h"


AudioAmbient::AudioAmbient(void)
{
	setSound(NULL);
	setGain(0.5f);
	setPitch(1.0f);
	setLooped(true);
}