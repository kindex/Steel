/*id*******************************************************************************
    Unit: Res Text
    Part of: Steel engine
    (C) DiVision, 2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        ������ ��� �������� ������ �� ���������� �����
**************************************************************************************/
#include "text_file.h"
#include "../../common/logger.h"
#include "../../common/utils.h"
#include "../rstream.h"

using namespace std;

bool TextFile::init(const std::string name, const std::string dir)
{
	std::string file = dir + "/" + name;

	rstream f(file); // no binary acces, only text
	if(f.fail()) 
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
	text = new unsigned char[size + 1];
	// get file data  
	pbuf->sgetn((char*)text, size);
	f.close();
 
	format = TEXT_ASCII;
	text[length] = '\0';

	return true;
}
