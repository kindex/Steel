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
#include "res/res_main.h"
#include "audio/openal_engine.h"

bool test()
{
	if (!initializeOpenAL())
		return false;

	Audio *tSnd = resAudio.add("audio/stereo");

	alSourcePlay(tSnd->itsSourceID);

	//destroyOpenAL();
	return true;
}
