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

#include "res/audio/wav.h"
#include "audio/openal_engine.h"

bool test()
{
	WAV *tSnd = new WAV;
	if (!initializeOpenAL())
		return false;

	tSnd->load("test.wav");	

	alSourcePlay(tSnd->itsSourceID);

	//destroyOpenAL();
	return true;
}
