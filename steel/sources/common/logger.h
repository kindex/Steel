/*id*********************************************************
	File: common/logger.h
	Unit: logger
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		Лог-файл
 ************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <fstream>

#include "../steel.h"
#include "timer.h"
#include "svector.h"

struct GlobalErrors
{
	int errorCount;
	int warningCount;

	GlobalErrors(): errorCount(0), warningCount(0) {}
};

extern GlobalErrors globalErrors;



#undef assert

/*
	проверка истиности выражения expression во время выполнения программы. 
	Если expression не выполняется, то выводится сообщение в файл и вызывается throw.
*/
#define assert(expression, message)		\
{										\
	if(!(expression))					\
	{									\
		log_msg("assert", message);		\
		throw;							\
	}									\
}

/*
	проверка условия во время компиляции. 
	Если условие не выполняется, то компиляция прерывается 
	с ошибкой «cannot allocate an array of constant size 0»
*/
#ifdef _DEBUG
	#define cassert(expression)	{	int __[(expression)?1:0];	}
#else
	#define cassert(expression)	{}
#endif

// именно этим макросом надо выводить все временные отладочные сообщения в лог файл.
#define debug(message)	log_msg("debug", (message))
#define debugi(message)	log_msg("debug", IntToStr(message))
#define debug_into(func, code) {  debug(" -> in  " + func);  code debug(" <- out " + func); }

void _log_msg(std::string keywords, std::string message);

// именно этим макросом надо выводить сообщения в лог-файл
#define log_msg(keywords, message)	_log_msg(keywords, message)

// именно этим макросом надо выводить сообщения в лог-файл
#define error(keywords, message) \
	globalErrors.errorCount++, \
	log_msg(std::string("error ") + \
		keywords, std::string(__FILE__) + "@" + \
		__FUNCTION__ + ":" + IntToStr(__LINE__) + " " + message) 

#define warn(keywords, message) \
	globalErrors.warningCount++, \
	log_msg(std::string("warning ") + \
		keywords, std::string(__FILE__) + "@" + \
		__FUNCTION__ + ":" + IntToStr(__LINE__) + " " + message)


// неудачный выход из процедуры с сообщением в лог
#define abort_init(keywords, _msg) { log_msg(keywords, _msg); return false; }

/*
	Класс для стандартного способа сообщения информации пользователю.
*/
class Logger
{
protected:
	// открыт ли файл для вывода сообщений
	bool opened;
	// файл, в который выводятся сообщения
	std::fstream f;
	// уровень вложенности сообщений
	int level;

public:
	Logger() { opened = false; level = 0; }

	//	Открытие файла лога и иницализация таймера, true в случае успеха.
	bool open(std::string filename);
	bool close();

/*
	Добавление сообщения в файл, перед текстом ставится время от начала открытия лог-файла. 
	Str – текст сообщения. 
	keywords – ключевые слова для быстрого поиска и отсеивания нужных записей в логе. 
	Ключевые слова отделяются пробелом. Обычно ключевое слово это имя модуля или функции, 
	откуда пришло сообщение.
*/
	void msg(std::string keywords, std::string message);

	// вывод без ключевых слов
	void out(std::string str);
	void push() { level++;}
	void pop(){ level--;}
	void setLevel(int _level) { level = _level; }
	int getLevel() { return level;}
};


#include <math.h> // для избежания конфликта имён с math.h::log()

// глобальный экземпляр класса steel::log, через который следует выводить все сообщения
namespace steel
{
	extern Logger log;
}


/* 
	LogFilter отвечает за отсеивание лишних сообщений в главном лог-файле. 
	Фильтр – это набор ключевых слов, отледённый пробелами. 
	Правила фильтра проверябтся посдедовательно. Если с сообщении есть ключевое слово 
	из фильтра, то оно выводиться, если же фильтр начинается с -, то отбрасывается. 
	Например, если фильтр “error -info”, то будут выводиться все сообщения с error, 
	а если error нет, то будут отбрасываться все сообщения с info
*/
class LogFilter
{
private:
	struct filterItem
	{
		std::string keyword;
		bool action;
	};
	svector<filterItem> filters;

public:
	// установить фильтр
	void set(std::string filter);
	// пропускает ли этот фильтр набор клювевый фраз
	bool check(std::string keywords);
};

extern LogFilter logFilter;

#endif
