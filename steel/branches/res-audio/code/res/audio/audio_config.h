/*id*********************************************************
	Unit: Audio Engine - Audio Config
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.09.15 21:50:42] Kuzmi4: File Creation.
    Description:
		Парсер config-файла.
 ************************************************************/

#include <vector>
#include <string>


#ifndef __RES_AUDIO_CONFIG_H
#define __RES_AUDIO_CONFIG_H

class AudioConfig
{
public:
	std::string name;
	std::string fileName;
	std::string type;
	float gain;
	float pitch;
	std::string load;
	std::string loop;
};

std::vector<AudioConfig> loadConfig();


#endif