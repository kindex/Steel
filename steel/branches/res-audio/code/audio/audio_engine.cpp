/*id*********************************************************
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.09.09 11:34:29] Kuzmi4: File Creation.
    Description:
		Звуковой движок.
 ************************************************************/

#include "audio_engine.h"

Listener::Listener()
{
	/*
	position = { 0.0f, 0.0f, 0.0f };
	velocity = { 0.0f, 0.0f, 0.0f };
	orientation = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	*/
	setPosition(0.0f, 0.0f, 0.0f);
	setVelocity(0.0f, 0.0f, 0.0f);
	setOrientation(v3(0.0f, 0.0f, -1.0f), v3(0.0f, 1.0f, 0.0f));
};