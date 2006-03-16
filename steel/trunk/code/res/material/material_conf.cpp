#include "material_conf.h"
#include "../../common/logger.h"
#include <fstream>

bool MaterialConf::init(const std::string name, ResLocatorArray &loadBefore, ResLocatorArray &loadAfter)
{
	std::string file = "../res/" + name + ".mat";

	rstream f(file, 0); // no binary acces, only text
	if(!f.good()) 
	{
		alog.out("Res/Material/Conf: cannot open file %s", file.c_str());
		return false;
	}

	f >> diffuse;
	
	loadAfter.push_back(ResLocator(Res::image, diffuse));

	return true;
}
