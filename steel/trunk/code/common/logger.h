/***************************************************************************
 *            logger.h
 *
 *  Fri Feb 10 11:36:51 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <stdio.h>

class Logger
{
public:
	/*
		Открытие и закрытие файла лога,
		true в случае успеха
	*/
	bool open( char *filename );
	bool open( std::string filename );
	bool close( void );
	
	/*
		Запись в файл, перед текстом ставится дата и время
	*/
	void out( char *fmt, ... );
	void out( std::string str );

	void msg(std::string keywords, std::string str);


private:
	FILE *f;
};

extern Logger alog;

#endif
