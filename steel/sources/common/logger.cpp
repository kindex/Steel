/*id*********************************************************
	File: common/logger.cpp
	Unit: logger
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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
#include "timer.h"
#include <sstream>
#include "containers.h"
#include "../objects/console.h"

#if STEEL_OS == OS_WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include <time.h>
#include <map>
#include <iomanip>
#include <set>

using namespace std;

bool Logger::open(std::string filename)
{
	if (opened)
    {
        close();
    }

	f.open(filename.c_str(), ios_base::out);

	if (f.bad())
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
    {
		return false;
    }
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
	if (!opened)
    {
        return;
    }

    std::stringstream s;

	steel::time total = globalTimer.total();
	
	s.fill('0');	s.width(2); 	s << (int)total << ".";
	s.fill('0');	s.width(3); 	s << (int)(total*1000)%1000;
 
	s.width(0);

	std::string levels;
	for(int i = 0; i < level; i++)
    {
		levels += "  ";
    }

	s << levels;

	s << " " << str;

    std::string output = s.str();

	if (!keywords.empty())
	{
		for (size_t i = output.length()/8; i < 9; i++)
        {
			output += '\t';
        }

		output += "\t[ " + keywords + " ]";
	}
	f << output << endl;
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
    std::set<std::string> keys_set;

	for EACH(svector<string>, keys, it)
	{
		string key = *it;
        keys_set.insert(key);
		if (loggers.find(key) == loggers.end())
		{
			loggers[key] = new Logger;
			loggers[key]->open("../log/" + key + ".log");
		}
		loggers[key]->setLevel(steel::log.getLevel());
		loggers[key]->msg(keywords, message);
	}

	if (logFilter.check(keywords))
    {
		steel::log.msg(keywords, message);
    }

    if (present("error", keys_set))
    {
        console.write(message, color4f(1, 0, 0));
    }
    else if (present("warning", keys_set))
    {
        console.write(message, color4f(1, 1, 0));
    }
}

void LogFilter::set(std::string filter)
{
	svector<string> keys;
	explode(' ', filter, keys);
	for EACH(svector<string>, keys, it)
	{
		filterItem item;

		string k = *it;
		if (k.empty()) continue;

		item.action = true;
		if (k[0] == '-') item.action = false;

		if (k[0] == '-') k.erase(0, 1);
		if (k[0] == '+') k.erase(0, 1);

		item.keyword = k;

		filters.push_back(item);
	}

}

bool LogFilter::check(std::string keywords)
{
	svector<string> keys;
	explode(' ', keywords, keys);

	for EACH(svector<filterItem>, filters, it)
	{
		for EACH(svector<string>, keys, jt)
        {
			if (it->keyword == *jt)
			{
				return it->action;
			}
        }
	}
	return true;
}

LogFilter logFilter;
GlobalErrors globalErrors;

