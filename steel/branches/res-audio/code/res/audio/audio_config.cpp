/*id*********************************************************
	Unit: Audio Engine - Audio Config
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.09.16 11:37:16] Kuzmi4: File Creation.
    Description:
		Парсер config-файла.
 ************************************************************/

#include "audio_config.h"
#include "../rstream.h"
#include "../../common/logger.h"

std::vector<AudioConfig> loadConfig()
{
	std::string strtemp = "";
	std::string dir = "audio";
	std::string name = "audio";
	std::string file = dir + "/" + name + ".conf";
	log_msg("audio res", "Opening audio config file...");
	rstream	f(file); 
	if (!f.good())
	{
		log_msg("audio res", "ERROR:  Cannot open audio config file!");
		//return 0;		// cannot open file
	}
	else
	{
		log_msg("audio res", "Audio config file - OK");
	}

	std::vector<AudioConfig> confVector;
	while (f.good())
	{
		AudioConfig conf;
		char buf[1024];
		f.getline(buf, 1023);
		std::string s = buf;	
		if (s.length() <= 2)
			continue;
		if((s.substr(0,2) == "//" )|| (s[0] == ';') || (s[0] == '#'))
			continue;
		strtemp = s.substr(0, s.find('\t')); conf.name = strtemp; // name
		s.erase(0, s.find('\t') + 1);
		strtemp = s.substr(0, s.find('\t')); conf.fileName = strtemp; // file
		s.erase(0, s.find('\t') + 1);
		strtemp = s.substr(0, s.find('\t')); conf.type = strtemp; // type
		s.erase(0, s.find('\t') + 1);
		strtemp = s.substr(0, s.find('\t')); conf.gain = (float) atof(strtemp.data()); // gain
		s.erase(0, s.find('\t') + 1);
		strtemp = s.substr(0, s.find('\t')); conf.pitch = (float) atof(strtemp.data()); // pitch
		s.erase(0, s.find('\t') + 1);
		strtemp = s.substr(0, s.find('\t')); conf.load = strtemp; // load
		s.erase(0, s.find('\t') + 1);
		strtemp = s.substr(0, s.find('\t')); conf.loop = strtemp; // loop
		s.erase(0, s.find('\t') + 1);
		confVector.push_back(conf);
	}
	return confVector;
}