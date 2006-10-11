/*id*********************************************************
	File: res/config/config_parser.h
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Парсер конфигов
	TODO:
		custom parser functions
		config templates, tags
 ************************************************************/

#ifndef __RES__CONFIG_PARSER_H
#define __RES__CONFIG_PARSER_H

#include "../../steel.h"

#include "../res.h"
#include "../text/text.h"
#include "config.h"

#define LOG_PARSE_ERROR(message)  \
	errors.push_back(ParseError(std::string(message), std::string(__FUNCTION__), line, charNumber));

class ConfigParser
{
public:
	ConfigParser(void): file(NULL) {}

	Config*	Parse(Text *file);

	Config*			ParseConfig(void);
	Config*			ParseValue(void);
	
	static double	getNumber(const char *text, int &position);
	double			ParseNumber(void);
	
	static std::string	getAlpha(const char *text, int &position);
	std::string		ParseAlpha(void);
	std::string		ParseString(void);

	void			SkipSpaces(void);
	ConfigStruct*	ParseStruct(void);
	ConfigArray*	ParseArray(void);

	static bool isFirstAlpha(char c);
	static bool isDigit(char c);

protected:
	char getc(void);
	void ungetc(void);
	char seec(void);

	struct ParseError
	{
		std::string message;
		std::string sender;
		int line, charNumber;

		ParseError(void) {}
		ParseError(std::string _message, std::string _sender, int _line, int _charNumber): message(_message), sender(_sender), line(_line), charNumber(_charNumber) {}
	};

	Text *file;
	int position;
	int line, charNumber, prevLine, prevCharNumber;
	svector<ParseError> errors;
};


#endif
