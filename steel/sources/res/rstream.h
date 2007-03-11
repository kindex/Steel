/*id*******************************************************************************
	File: res/rstream.h
	Unit: rstream
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        Класс и интерфейсом ifstream для загрузки игровых ресурсов
**************************************************************************************/

#ifndef __RES_RSTREAM_H
#define __RES_RSTREAM_H

#include "../steel.h"
#include <fstream>

/*
Resourse input file stream
Все рагрузки ресурсов из файловой системы должны использовать этот класс для чтения из файлов
От него наследуются загрузки из файла, архива или скачивания из инета.
*/
class rstream: public std::ifstream
{
protected:
	bool failed;

public:
	rstream(): failed(true) {}
	rstream(const std::string& s, const std::string& ext = "", ios_base::openmode _Mode = std::ios::binary) 
	{ 
		open(s, ext, _Mode);
	}
	
	bool fail() { return failed || std::ifstream::fail(); }
	bool open(const std::string& s, const std::string& ext = "", ios_base::openmode _Mode = std::ios::binary);

	void read(void* dest, int size);
	void skip(int n);// skip n byten in input stream
};

#endif
