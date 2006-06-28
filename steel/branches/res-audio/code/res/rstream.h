/*id*******************************************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        ћолуль дл€ хранени€, загрузки игровых ресурсов и контрол€ над ними. 
	Parts:
		res.cpp
		¬ поддиректори€ хран€тс€ классы дл€ загрузки и хранени€ всех типов ресурсов.
**************************************************************************************/

#ifndef __RES_RSTREAM_H
#define __RES_RSTREAM_H

#include <fstream>


/*
Resourse input file stream
¬се рагрузки ресурсов из файловой системы должны использовать этот класс дл€ чтени€ из файлов
ќт него наследуютс€ загрузки из файла, архива или скачивани€ из инета.
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
