/*id*********************************************************
	File: res/image/jpeg.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		JPEG loader (using NV_UTIL lib)
 ************************************************************/

#include "../../steel.h"
#include "jpeg.h"
#include "../../common/logger.h"
#include "../rstream.h"
#include <istream>
#include <jpeg/jpeglib.h>
#include <jpeg/jerror.h>
#include <string> 
#include <setjmp.h>

Image* createJPEG(const std::string& filename)
{
	ResJPEG *o = new ResJPEG;
	if (o->init(filename))
	{
		o->setId(objectIdGenerator.genUid());
		return o;
	}
	else
	{
		delete o;
		return NULL;
	}
}



struct image
{
	long width;
	long height;
	void* pImgData;
};

struct my_error_mgr {
//! fields used for jpeg error handling (this MUST BE FIRST MEMBER so libjpeg can treat my_error_mgr as a jpeg_error_mgr struct — C-style "inheritance")
struct jpeg_error_mgr pub;
char buffer[JMSG_LENGTH_MAX]; //!< message indicating reason for error
jmp_buf setjmp_buffer;  //!< a jump buffer to trigger on error, similar to exception except not as safe (use a C library, this is what you get...)
};

typedef struct my_error_mgr * my_error_ptr;

 /*  
  * Here's the routine that will replace the standard error_exit method:  
  */   
    
static void my_error_exit (j_common_ptr cinfo)   
{   
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */   
	my_error_ptr myerr = (my_error_ptr) cinfo->err;   

	/* Always display the message. */   
	/* We could postpone this until after returning, if we chose. */   
	(*cinfo->err->output_message) (cinfo);   

	/* Return control to the setjmp point */   
	longjmp(myerr->setjmp_buffer, 1);   
}

/*
* This is working test method, we keep it until jpeg is tested
*/
bool LoadJPG( const char* pFilename, image& img )
{
// this is original fopen routine, we use rstream instead
//	FILE *pFile=NULL;

	/*if ((pFile = fopen(pFilename, "rb")) == NULL)
		return FALSE;
*/
	struct jpeg_decompress_struct cinfo;

	my_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return FALSE;
	}

	rstream f;

	if(!f.open(pFilename, "jpg"))
	{
		log_msg("warning res image tga", std::string("Res/Image/TGA: cannot open file ")+ pFilename);
		return false;
	}


	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, &f);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	int stride = cinfo.output_width * cinfo.output_components;
	unsigned char** buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, stride, 1);

	img.width = cinfo.output_width;
	img.height = cinfo.output_height;
	img.pImgData = (void*)new unsigned long[cinfo.output_width*cinfo.output_height];

	unsigned long* pixPtr = (unsigned long*)img.pImgData;
	const int a = 255;

	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
		unsigned char* pcomp = *buffer;
		for (unsigned int i = 0; i < cinfo.output_width; i++) {
			int r = *pcomp++;
			int g = *pcomp++;
			int b = *pcomp++;
			*pixPtr ++ = (a<<24) | (r << 16) | (g << 8) | (b);
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	f.close();

//	fclose(pFile);
	return TRUE;
}

bool ResJPEG::init(const std::string& name)
{  

	rstream f;

	if(!f.open(name, "jpg"))
	{
		log_msg("warning res image tga", std::string("Res/Image/TGA: cannot open file ")+ name.c_str());
		return false;
	}

	struct jpeg_decompress_struct cinfo;

	my_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		f.close();
		return FALSE;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, &f);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	int stride = cinfo.output_width * cinfo.output_components;
	unsigned char** buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, stride, 1);

	createImage(cinfo.output_width, cinfo.output_height, 24);

	unsigned char* pBitmap = bitmap;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
		unsigned char* pcomp = *buffer;

		// in first implementation we move all bytes manually. if this will work, we will replace with memcpy
		for (unsigned int i = 0; i < cinfo.output_width; i++) {
			*pBitmap=*pcomp;
			pcomp++;
			pBitmap++;
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	dimension	= IMAGE_2D;
	format = IMAGE_RGB;
	return true;
}
