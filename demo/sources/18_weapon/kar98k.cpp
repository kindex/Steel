/*id*********************************************************
	File: kar98k.cpp
	Unit: ??
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
#include <res/res_main.h>
#include <audio/sound.h>

Kar98k::Kar98k()
{
	caliber = 7.92f;	// mm
	velocity = 755;		// m/s
	rate = 15;			// rounds per minute
	magazine = 5;		// rounds
	weight = 3900;		// g
	length = 1.11f;		// m
	
	kar98k = new AudioList();
	kar98k->audio["fire"] = new SimpleSound();
	Sound* temp = new Sound();
	temp->sound = resAudio.add("audio/kar98k/wpnfire_k98_1b");
	kar98k->audio["fire"]->originalSound = temp;
	/*sndFire = new Sound();
	sndFire->sound = resAudio.add("audio/kar98k/wpnfire_k98_1b");
	sndReload = new Sound();
	sndReload->sound = resAudio.add("audio/kar98k/kar98_reload");
	sndRechamber = new Sound();
	sndRechamber->sound = resAudio.add("audio/kar98k/kar98_rechamber");
	kar98k = new SimpleSound();*/
}

void Kar98k::fire()
{
	magazine--;
	kar98k->audioPlay("fire");
	kar98k->soundPlay(kar98k->audio["fire"]->originalSound);
	//	auFire->setStatus("play");
	// fireSound play
}

void Kar98k::reload()
{
	if (magazine != 5)
	{
		magazine = 5;
//		auReload->setStatus("play");
		
		// reloading sound play
		kar98k->soundPlay(kar98k->audio["reload"]->originalSound);
	}
}

void Kar98k::rechamber()
{
	kar98k->soundPlay(kar98k->audio["rechamber"]->originalSound);
}