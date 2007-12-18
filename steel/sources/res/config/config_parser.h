/*id*********************************************************
	File: res/config/config_parser.h
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Парсер конфигов
	TODO:
		custom parser functions
 ************************************************************/

#ifndef __RES__CONFIG_PARSER_H
#define __RES__CONFIG_PARSER_H

#include "../../steel.h"

#include "../res.h"
#include "../text/text.h"
#include "config.h"

#define LOG_PARSE_ERROR(message)  \
	errors.push_back(ParseError(std::string(message), std::string(__FUNCTION__), line, charNumber))

Config* parseConfig(const std::string& text);

struct ConfigTag;

class ConfigParser
{
public:
	ConfigParser(): file(NULL) {}

	Config*	Parse(Text *file);

	Config*			ParseConfig();
	ConfigTag*		ParseTag();
	Config*			ParseValue();
	
	static double	getNumber(const char *text, int &position);
	double			ParseNumber();
	Config*			ParseNull();
	
	static std::string	getAlpha(const char *text, int &position);
	std::string		ParseAlpha();
	std::string		ParseString();

	void			SkipSpaces();
	ConfigStruct*	ParseStruct();
	ConfigArray*	ParseArray();

	static bool isFirstAlpha(char c);
	static bool isDigit(char c);

protected:
	char getc();
	void ungetc();
	char seec();

	struct ParseError
	{
		std::string message;
		std::string sender;
		int line, charNumber;

		ParseError() {}
		ParseError(std::string _message, std::string _sender, int _line, int _charNumber): message(_message), sender(_sender), line(_line), charNumber(_charNumber) {}

		std::string getMessage();
	};

	Text *file;
	int position;
	int line, charNumber, prevLine, prevCharNumber;
	svector<ParseError> errors;

	friend class ConfigParserTest;
};

struct ConfigTag
{
	svector<ConfigTemplate> templates;
};

#endif
