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
		Лог-файл
 ************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "timer.h"

#include <string>
#include <fstream>

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
#define cassert(expression)	{	int __[(expression)?1:0];	}

// именно этим макросом надо выводить все временные отладочные сообщения в лог файл.
#define debug(message)	log_msg("debug", message)

// именно этим макросом надо выводить сообщения в лог-файл
#define log_msg(keywords, message)	steel::log.msg(keywords, message)

// неудачный выход из процедуры с сообщением в лог
#define abort_init(keywords, _msg) { log_msg(keywords, _msg); return false; }

/*
	Класс для стандартного способа сообщения информации пользователю.
*/
class Logger
{
protected:
	bool opened;
	std::fstream f;
	Timer timer;

public:
	Logger() { opened = false; }

	//	Открытие файла лога и иницализация таймера, true в случае успеха.
	bool open(std::string filename);
	bool close(void);

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
};


#include <math.h> // для избежания конфликта имён с math.h::log()

// глобальный экземпляр класса steel::log, через который следует выводить все сообщения
namespace steel
{
	extern Logger log;
}

#endif
