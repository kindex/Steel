/*id*********************************************************
	File: common/logger.cpp
	Unit: logger
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Лог-файл
 ************************************************************/

#include "../steel.h"

#include "logger.h"
#include "utils.h"

#if STEEL_OS == OS_WIN32
#include <windows.h>
#endif

#include <time.h>
#include <map>
#include <iomanip>

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

	f << "Started Logging @ [ ";

	//Def
	//read SystemTime
	//This is Systesm Time Insertion Into Log File Header
	#if STEEL_OS == OS_WIN32
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	f 
		<< sm.wYear << "-" 
		<< setfill('0') << setw(2) << sm.wMonth << "-"
		<< setfill('0') << setw(2) << sm.wDay << " "
		<< setfill('0') << setw(2) << sm.wHour << ":"
		<< setfill('0') << setw(2) << sm.wMinute << ":"
		<< setfill('0') << setw(2) << sm.wSecond << "."
		<< setfill('0') << setw(3) << sm.wMilliseconds << " ]"<< endl;

	#elif STEEL_OS == OS_LINUX
	time_t sm = ::time(NULL);
	struct tm *tm = localtime(&sm);
	f << tm->tm_mday << "." << tm->tm_mon << "." << 1900+tm->tm_year << " " \
		<< tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "]" << endl;
	#endif

	return true;
}

bool Logger::close( void )
{
	if(opened)
	{
		out("Finished Logging");
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

	steel::time total = globalTimer.total();
	
	f.fill('0');	f.width(2); 	f << (int)total/60 << ":";
	f.fill('0');	f.width(2); 	f << (int)total%60 << ".";
	f.fill('0');	f.width(3); 	f << (int)(total*1000)%1000;
 
	f.width(0); 

	std::string levels;
	for(int i=0; i<level; i++)
		levels += "  ";

	f << levels;

	f << " " << str;

	if(!keywords.empty())
	{
		for(int i = 0; i < 6 - ((int)str.length() + 2 + level*2)/8; i++)
			f << "\t";

		f << "\t[ " << keywords << " ]";
	}
	f << endl;
}

namespace steel
{
	Logger log;
}

map<string, Logger*> loggers;

void _log_msg(string keywords, string message)
{
	svector<string> keys;
	explode(' ', keywords, keys);
	for(svector<string>::iterator it = keys.begin(); it != keys.end(); it++)
	{
		string key = *it;
		if(loggers.find(key) == loggers.end())
		{
			loggers[key] = new Logger;
			loggers[key]->open("../log/" + key + ".log");
		}
		loggers[key]->setLevel(steel::log.getLevel());
		loggers[key]->msg(keywords, message);
	}
	if(logFilter.check(keywords))
		steel::log.msg(keywords, message);
}

void LogFilter::set(std::string filter)
{
	svector<string> keys;
	explode(' ', filter, keys);
	for(svector<string>::iterator it = keys.begin(); it!= keys.end(); it++)
	{
		filterItem item;

		string k = *it;
		if(k.empty()) continue;

		item.action = true;
		if(k[0] == '-') item.action = false;

		if(k[0] == '-') k.erase(0, 1);
		if(k[0] == '+') k.erase(0, 1);

		item.keyword = k;

		filters.push_back(item);
	}

}

bool LogFilter::check(std::string keywords)
{
	svector<string> keys;
	explode(' ', keywords, keys);

	for(svector<filterItem>::iterator it = filters.begin(); it!= filters.end(); it++)
	{
		for(svector<string>::iterator jt = keys.begin(); jt!= keys.end(); jt++)
			if(it->keyword == *jt)
			{
				return it->action;
			}
	}
	return true;
}

LogFilter logFilter;
GlobalErrors globalErrors;

