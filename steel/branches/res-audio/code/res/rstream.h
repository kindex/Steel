/*id*******************************************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        ������ ��� ��������, �������� ������� �������� � �������� ��� ����. 
	Parts:
		res.cpp
		� ������������� �������� ������ ��� �������� � �������� ���� ����� ��������.
**************************************************************************************/

#ifndef __RES_RSTREAM_H
#define __RES_RSTREAM_H

#include <fstream>


/*
Resourse input file stream
��� �������� �������� �� �������� ������� ������ ������������ ���� ����� ��� ������ �� ������
�� ���� ����������� �������� �� �����, ������ ��� ���������� �� �����.
*/
class rstream: public std::ifstream
{
public:
	rstream() {}
	rstream(std::string s, std::string ext = "", ios_base::openmode _Mode = std::ios::binary) 
	{ 
		open(s, ext, _Mode);
	}

	bool open(std::string s, std::string ext = "", ios_base::openmode _Mode = std::ios::binary);

	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};

#endif
