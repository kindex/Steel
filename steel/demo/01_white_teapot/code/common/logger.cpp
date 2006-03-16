/***************************************************************************
 *            logger.cpp
 *
 *  Fri Feb 10 11:46:32 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <time.h>
#include <stdarg.h>

#include "logger.h"

Logger alog;

bool Logger::open( char *filename )
{
	f=fopen(filename,"a");
	if ( f==NULL )
	{
		return false;
	}
	fprintf(f,"\n>>>>>>>>>>>>>>>>\n");
	Logger::out("Started Logging\n");
	return true;
}

bool Logger::open( std::string filename )
{
	return false;
}

bool Logger::close( void )
{
	Logger::out("Finished Logging\n");
	fprintf(f,"<<<<<<<<<<<<<<<<\n");
	if ( fclose(f)==-1 )
		return false;
	return true;
}

void Logger::out( char *fmt, ... )
{
	va_list ap;
	time_t ct;
	struct tm *tt;
	time(&ct);
	tt=localtime(&ct);
	va_start(ap,fmt);
	fprintf(f,"[%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ", \
			tt->tm_mday,tt->tm_mon,1900+tt->tm_year, \
			tt->tm_hour,tt->tm_min,tt->tm_sec);
	vfprintf(f,fmt,ap);
	#ifdef DEBUG
	printf("[%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ", \
			tt->tm_mday,tt->tm_mon,1900+tt->tm_year, \
			tt->tm_hour,tt->tm_min,tt->tm_sec);
	vprintf(fmt,ap);
	#endif
	va_end(ap);
}

void Logger::out( std::string str )
{
	
}
