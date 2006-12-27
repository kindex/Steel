/*id*********************************************************
	File: objects/kar98k.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Karabiner 98k Rifle		
 ************************************************************/

#include "kar98k.h"
#include "../res/res_main.h"

Kar98k::Kar98k(void)
{
	caliber = 7.92f;	// mm
	velocity = 755;		// m/s
	rate = 15;			// rounds per minute
	magazine = 5;		// rounds
	weight = 3900;		// g
	length = 1.11f;		// m
	sndFire = new Sound();
	sndFire->sound = resAudio.add("audio/kar98k/wpnfire_k98_1b");
	sndReload = new Sound();
	sndReload->sound = resAudio.add("audio/kar98k/kar98_reload");
	sndRechamber = new Sound();
	sndRechamber->sound = resAudio.add("audio/kar98k/kar98_rechamber");
	kar98k = new SimpleSound();
}

void Kar98k::fire(void)
{
	magazine--;
	kar98k->soundPlay(sndFire);
//	auFire->setStatus("play");
	// fireSound play
}

void Kar98k::reload(void)
{
	if (magazine != 5)
	{
		magazine = 5;
//		auReload->setStatus("play");
		
		// reloading sound play
	}
}

void Kar98k::rechamber(void)
{

}