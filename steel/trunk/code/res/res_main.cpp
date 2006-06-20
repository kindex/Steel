#include "res_main.h"

#include "image/bmp.h"
#include "image/tga.h"
#include "model/_3ds.h"
#include "conf/conf_text.h"
#include "script/script_text.h"


ResCollection<Config> resConfig;
ResCollection<Image> resImage;
ResCollection<Model> resModel;
ResCollection<Material> resMaterial;
ResCollection<Script> resScript;



// Image filename:
// name[.cube][.normal|.height][.bmp|.tga]

Image *createBitmap(const std::string filename)
{
	Image *r;
	// try to load image2d
	if(r = createBMP(filename)) return r;
	if(r = createTGA(filename)) return r;
	return NULL;
}

Image *createImageFormat(const std::string filename)
{
	Image *r;
	// try to load image2d
	if(r = createBitmap(filename))	return r;
	if(r = createBitmap(filename + ".normal"))	
	{
		((Image*)r)->setFormat(ImageFormat::normal);
		return r;
	}
	if(r = createBitmap(filename + ".height"))
	{
		((Image*)r)->convertFromHeightMapToNormalMap();
		((Image*)r)->setFormat(ImageFormat::normal);
		return r;
	}

	return NULL;
}


Image *createImage(const std::string filename)
{
	Image *r;
	// try to load image2d
	if(r = createImageFormat(filename)) return r;
	// try to load cubemap
	if(r = createImageFormat(filename + ".cube"))
	{
		((Image*)r)->setKind(ImageKind::cube);
		return r;
	}
	return NULL;
}


bool registerResources()
{
	resImage.registerClass(createImage);
	resModel.registerClass(create3DS);
	resMaterial.registerClass(createMaterial);
	resConfig.registerClass(createConfigText);
	resScript.registerClass(createScriptText);

	return true;
}
