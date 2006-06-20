/*id*********************************************************
    Unit: logger
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
    License:
        Steel Engine License
    Description:
		Ëמד-פאיכ
 ************************************************************/

#include <time.h>
#include <stdarg.h>

#include "logger.h"

using namespace std;

bool Logger::open(std::string filename)
{
	if(opened) close();

	f.open(filename.c_str(), ios_base::out);

	if(f.bad())
	{
		return false;
	}

	opened = true;

	timer.start();

	f << ">>>>>>>>>>>>>>>>" << endl;
	f << "Started Logging" << endl;

	return true;
}

bool Logger::close( void )
{
	if(opened)
	{
		out("Finished Logging");
		out("<<<<<<<<<<<<<<<<");
		f.close();
		opened = false;
		return true;
	}
	else
		return false;
}

/*void Logger::out( char *fmt, ... )
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
	fprintf(f,"\n");

	fflush(f);
}*/

void Logger::out(std::string str)
{
	msg("", str);
}


void Logger::msg(std::string keywords, std::string str)
{
	if(!opened) return;

	steel::time total = timer.total();
	
	f.fill('0');	f.width(2); 	f << (int)total/60 << ":";
	f.fill('0');	f.width(2); 	f << (int)total%60 << ".";
	f.fill('0');	f.width(3); 	f << (int)(total*1000)%1000;
 
	f.width(0); 

	f << "\t" << str;

	if(!keywords.empty())
	{
		for(unsigned int i = 0; i < 4 - (unsigned int)str.length()/8; i++)
			f << "\t";

		f << "\t[ " << keywords << " ]";
	}
	f << endl;
}

namespace steel
{
	Logger log;
}
