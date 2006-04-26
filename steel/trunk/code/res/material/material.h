#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../image/image.h"
#include "../conf/conf.h"
#include "../script/script.h"

class Map
{
public:
	Image *texture;
	typedef	enum
	{
		none,
		replace,
		add,
		mul

	} map_mode;

	typedef	enum
	{
		_none,
		color_map,
		normal_map,
		diffuse_lights // procedure texture
	} tex_kind;

	map_mode	mode;
	tex_kind	kind;
	// color
	// mode
	// shader
};

class Material: public Res
{
public:
	Script			*conf;
	std::vector<Map> map;

	bool init(std::string _conf, ResCollection *res);
	virtual bool unload() {return true; };

};

Res* createMaterial(const std::string filename, ResCollection *res);


#endif
