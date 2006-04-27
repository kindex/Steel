#ifndef __RES__MATERIAL_H
#define __RES__MATERIAL_H

#include "../image/image.h"
#include "../conf/conf.h"
#include "../script/script.h"

#include "../../math/vector4d.h"

namespace MapMode
{
	typedef	enum
	{
		none,
		replace,
		add,
		mul
	} map_mode;
}

namespace MapKind
{
	typedef	enum
	{
		none,
		color_map,
		color,
		normal_map,
		diffuse_lights // procedure texture
	} tex_kind;
}

class Map
{
public:
	Image *texture;


	MapMode::map_mode	mode;
	MapKind::tex_kind	kind;
	v4					color;
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
