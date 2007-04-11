/*id*********************************************************
	File: kar98k.h
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

#ifndef KAR98K_H
#define KAR98K_H

#include <objects/combiner/combiner.h>
#include <objects/combiner/audio/simple_sound.h>
#include <objects/combiner/audio/audio_list.h>

class Kar98k : public Combiner
{
public:
	Kar98k();
	//~Kar98k();
	void reload();
	void rechamber();
	void fire();
	AudioList* kar98k;	// must be AudioObject* kar98k; then smth like  = new AudioList() ...
	//GraphObject* ...
	//PhysObject* ...

private:
	float caliber;		// = 7.92 mm;
	float velocity;		// muzzle velocity = 2477 feet per second
	int rate;			// rate of fire = 15 rounds per minute
	int magazine;		// magazine = 5 rounds
	float weight;		// weight = 3900 g
	float length;		// length = 1.11 m

	//AudioObject* kar98k;
	//Sound* sndRechamber;
	//Sound* sndReload;
	//Sound* sndFire;
};

#endif
