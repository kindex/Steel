#include "res_main.h"

#include "image/bmp.h"
#include "image/tga.h"
#include "model/_3ds.h"
#include "conf/conf_text.h"
#include "script/script_text.h"
#include "text/text_file.h"


ResCollection<Config>	resConfig;
ResCollection<Image>	resImage;
ResCollection<Model>	resModel;
ResCollection<Material> resMaterial;
ResCollection<Script>	resScript;
ResCollection<Text>		resText;


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
		((Image*)r)->setFormat(IMAGE_NORMAL);
		return r;
	}
	if(r = createBitmap(filename + ".height"))
	{
		((Image*)r)->convertFromHeightMapToNormalMap();
		((Image*)r)->setFormat(IMAGE_NORMAL);
		return r;
	}

	return NULL;
}


Image *createImage(const std::string filename, const std::string base)
{
	Image *r;
	// try to load image2d
	if(r = createImageFormat(base + "/" +  filename)) return r;
	// try to load cubemap
	if(r = createImageFormat(base + "/" + filename + ".cube"))
	{
		((Image*)r)->setDimension(IMAGE_CUBE);
		return r;
	}
	return NULL;
}



template<class T, class T2>
T2* createClass(const std::string filename, const std::string base)
{
	if(filename.empty()) return false;

	T* object = new T;

	if(object->init(filename, base))
	{
		object->setId(objectIdGenerator.genUid());
		return object;
	}
	else
	{
		delete object;
		return NULL;
	}
}

bool registerResources()
{
	resImage.setId("image");	resImage.registerResLoader(createImage);
	resModel.setId("model");	resModel.registerResLoader(createClass<_3DS, Model>);
	resMaterial.setId("material");resMaterial.registerResLoader(createClass<Material, Material>);
	resConfig.setId("config");	resConfig.registerResLoader(createClass<ConfigText, Config>);
	resScript.setId("script");	resScript.registerResLoader(createClass<ScriptText, Script>);
	resText.setId("text");	resText.registerResLoader(createClass<TextFile, Text>);

	return true;
}
