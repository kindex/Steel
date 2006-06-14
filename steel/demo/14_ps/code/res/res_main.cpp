#include "res_main.h"

#include "image/bmp.h"
#include "image/tga.h"
#include "model/_3ds.h"
#include "conf/conf_text.h"
#include "script/script_text.h"


// Image filename:
// name[.cube][.normal|.height][.bmp|.tga]

Res *createBitmap(const std::string filename, ResCollection *res)
{
	Res *r;
	// try to load image2d
	if(r = createBMP(filename, res)) return r;
	if(r = createTGA(filename, res)) return r;
	return NULL;
}

Res *createImageFormat(const std::string filename, ResCollection *res)
{
	Res *r;
	// try to load image2d
	if(r = createBitmap(filename, res))	return r;
	if(r = createBitmap(filename + ".normal", res))	
	{
		((Image*)r)->setFormat(ImageFormat::normal);
		return r;
	}
	if(r = createBitmap(filename + ".height", res))
	{
		((Image*)r)->convertFromHeightMapToNormalMap();
		((Image*)r)->setFormat(ImageFormat::normal);
		return r;
	}

	return NULL;
}


Res *createImage(const std::string filename, ResCollection *res)
{
	Res *r;
	// try to load image2d
	if(r = createImageFormat(filename, res)) return r;
	// try to load cubemap
	if(r = createImageFormat(filename + ".cube", res))
	{
		((Image*)r)->setKind(ImageKind::cube);
		return r;
	}
	return NULL;
}


bool registerResources(ResCollection &res)
{
	res.registerClass(createImage,		Res::image);
	res.registerClass(create3DS,		Res::model);
	res.registerClass(createMaterial,	Res::material);
	res.registerClass(createConfigText,	Res::config);
	res.registerClass(createScriptText,	Res::script);
	return true;
}
