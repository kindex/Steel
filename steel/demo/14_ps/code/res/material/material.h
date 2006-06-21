#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../image/image.h"
#include "../conf/conf.h"
#include "../script/script.h"
#include "../../graph/types.h"

namespace MapMode
{
	typedef	enum
	{
		none,
		replace,
		add,
		mul,
		blend
	} map_mode;
}

namespace MapKind
{
	typedef	enum
	{
		none,
		color_map,
		color,
		env,
		normal_map,
		bump_map,
		diffuse_lights // procedure texture
	} tex_kind;
}

class Map
{
public:
	Image *texture;

	MapMode::map_mode	mode;
	MapKind::tex_kind	kind;
	color4f				color;
	// color
	// mode
	// shader
};

class Material: public Res
{
public:
	Script			*conf;
	steel::vector<Map> map;
	bool				blend;

	bool init(std::string _conf, ResCollection *res);
	virtual bool unload() {return true; };
};

Res* createMaterial(const std::string filename, ResCollection *res);

#endif