/*id*******************************************************************************
	File: res/text/text_file.cpp
	Unit: res/text
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Молуль для рагрузки текста из текстового файла
**************************************************************************************/

#include "../../steel.h"
#include "text_file.h"
#include "../../common/logger.h"
#include "../../common/utils.h"
#include "../rstream.h"

using namespace std;

bool TextFile::init(const std::string& name, const std::string dir)
{
	std::string file = dir + "/" + name;
	this->filename = file;

	rstream f(file); // no binary acces, only text
	if (f.fail()) 
	{
		log_msg("res conf error", "Cannot open file " + file);
		return false;
	}

	filebuf *pbuf;
	long size;

	pbuf = f.rdbuf();
	// get file size using buffer's members
	size = pbuf->pubseekoff (0,ios::end,ios::in);
	length = size;
	pbuf->pubseekpos (0,ios::in);
	// allocate memory to contain file data
	text = new char[size + 1];
	// get file data  
	pbuf->sgetn(text, size);
	f.close();
 
	format = TEXT_ASCII;
	text[length] = '\0';

	return true;
}

char* readfile(std::string file)
{
	char *text;

	rstream f(file); // no binary acces, only text
	if(f.fail()) 
	{
		log_msg("res conf error", "Cannot open file " + file);
		return NULL;
	}

	filebuf *pbuf;
	long size;

	pbuf = f.rdbuf();
	// get file size using buffer's members
	size = pbuf->pubseekoff (0,ios::end,ios::in);
	pbuf->pubseekpos (0,ios::in);
	// allocate memory to contain file data
	text = new char[size + 1];
	// get file data  
	pbuf->sgetn((char*)text, size);
	f.close();
 
	text[size] = '\0';
	return text;
}
