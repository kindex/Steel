/*id*********************************************************
	File: res/config/config_parser.cpp
	Unit: res config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Парсер конфигов
 ************************************************************/

#include "../../steel.h"

#include "config_parser.h"
#include "../text/text_string.h"
#include <math.h>

using namespace std;

Config* parse(std::string str)
{
	Config *obj = NULL;
	TextString file;
	file.Setup(str);
	
	ConfigParser* parser = new ConfigParser();
	obj = parser->Parse(&file);
	delete parser;

	return obj;
}

Config*	ConfigParser::Parse(Text *_file)
{
	if(_file == NULL) return NULL;
	file = _file;
	position = 0;
	line = 1;
	charNumber = 1;
	errors.clear();
	Config *res = ParseConfig();

	if(res != NULL)
	{
		SkipSpaces();
		char c = seec();
		
		if(c != '\0') 
			LOG_PARSE_ERROR(string("Unxpected symbol '") + c + "'. Expecting EOF");
	}
	for(svector<ParseError>::iterator it = errors.begin(); it != errors.end(); it++)
	{
		log_msg("error parser", it->message + 
			" (" +  IntToStr(it->line) + ":" + IntToStr(it->charNumber) + ") [" +
			it->sender + "]");
	}


	return res;
}

Config* ConfigParser::ParseConfig(void)
{
	SkipSpaces();
	char c = seec();
	ConfigTag *tag = NULL;
	if(c == '[') // found tag
	{
		tag = ParseTag();
	}
	if(c == '\0') // no value
	{
		LOG_PARSE_ERROR("No value in config");
		return NULL;
	}
	
	Config *result = ParseValue();
	if(tag != NULL)
	{
		if(result != NULL)
		{
			result->templates = tag->templates;
		}
		delete tag;
	}
	return result;
}


ConfigTag* ConfigParser::ParseTag()
{
	char c = seec();
	if(c != '[') return NULL;
	getc();
	string collector;
	int level = 0;

	ConfigTag * tag = NULL;
	for(;;)
	{
		c = getc();
		if(c == ']' && level == 0) break;
		else if(c == ',')
		{
			if(!collector.empty())
			{
				if(tag == NULL) tag = new ConfigTag;
				tag->templates.push_back(collector);
				collector.clear();
			}
			else
				LOG_PARSE_ERROR(string("Empty path to template"));
		}
		else if(c == '_' || c == '-' || c>='0' && c<='9' || c>='A' && c<='Z' || c>='a' && c<='z' || c =='[' || c == ']' || c == '.' || c == '#' || c == '/')
		{
			collector += c;
			if(c == '[')
				if(level >= 1)
					LOG_PARSE_ERROR(string("Too many ["));
				else
					level++;
			if(c == ']') 
				if(level <= 0)
					LOG_PARSE_ERROR(string("Too many ]"));
				else
					level--;
		}
		else LOG_PARSE_ERROR(string("Unxpected symbol '") + c + "'");
	}
	if(!collector.empty())
	{
		if(tag == NULL) 
		{
			tag = new ConfigTag();
		}
		tag->templates.push_back(collector);
	}

	return tag;
}


Config* ConfigParser::ParseValue()
{
	Config *res = NULL;

	SkipSpaces();
	char c = seec();
	if(c == '\0')
	{
		return res;
	}
		
	// Number
	if(c == '+' || c == '-' || (c>='0' && c <= '9'))
	{
		res = new ConfigNumber(ParseNumber());
	}
	else if(c == '\'' || c == '"')	// String
	{
		res = new ConfigString(ParseString());
	}
	else if(c == '{') // Struct
	{
		res = ParseStruct();
	}
	else if(c == '(') // Array
	{
		res = ParseArray();
	}
	else if(c == 'n') // null
	{
		res = ParseNull();
	}
	else
	{
		LOG_PARSE_ERROR(string("Illegal symbol '") + c + "'");
		getc(); // skip char to avoid infinite cycle
		return NULL;
	}
	return res;
}


string ConfigParser::ParseAlpha()
{
	string res;
	char c = getc();
	// skip spaces
	if(!isFirstAlpha(c))
	{
		LOG_PARSE_ERROR("Expecting alpha character");
	}
	while(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ||c == '_')
	{
		res.push_back(c);
		c = getc();
	}
	if(res.empty())
	{
		LOG_PARSE_ERROR("Empty AlphaString");
	}
	else
		ungetc();

	return res;
}

string ConfigParser::getAlpha(const char *text, int &position)
{
	string res;
	char c = text[position];
	// skip spaces
	if(!isFirstAlpha(c)) return res;
	
	while(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ||c == '_')
	{
		res.push_back(c);
		c = text[++position];
	}

	return res;
}


ConfigStruct* ConfigParser::ParseStruct(void)
{
	char c = getc();
	if(c != '{')
	{
		ungetc();
		LOG_PARSE_ERROR(string("Illegal symbol '") + c + "'");
		return NULL;
	}
	char end = '}';

	ConfigStruct *res = new ConfigStruct();
	for(;;)
	{
		Config* value = NULL;

		SkipSpaces();
		c = seec();
		if(c == end)
		{
			getc();
			return res;
		}
		if(c == '\0')
		{
			LOG_PARSE_ERROR(string("Unexpected end of config. Expecting '") + end + "'");
			return res;
		}
		string var;
		var = ParseAlpha();
		SkipSpaces();
		c = getc();
		if(c != '=') 
		{
			LOG_PARSE_ERROR(string("Illegal symbol '") + c + "'. Expecting '='");
			continue;
		}
		SkipSpaces();
		value = ParseConfig();
		if(value != NULL)
		{
			if(res->getStructElement(var) == NULL)
			{
				value->setParent(res);
				res->setValue(var, value);
			}
			else
			{
				LOG_PARSE_ERROR(string("Duplicate key '") + var + "'");
				delete value; value = NULL;
			}
		}
		else
			break;

		SkipSpaces();
		c = seec();
		if(c == ';') getc();		// optional ';'
	}

	return res;
}


ConfigArray* ConfigParser::ParseArray(void)
{
	char c = seec();
	if(c != '(')
	{
		LOG_PARSE_ERROR(string("Illegal symbol '") + c + "'");
		return NULL;
	}
	char end = ')';
	getc();

	ConfigArray *res = new ConfigArray();
	for(;;)
	{
		Config* value = NULL;

		SkipSpaces();
		c = seec();
		if(c == end) 
		{
			getc();
			return res;
		}
		if(c == '\0')
		{
			LOG_PARSE_ERROR(string("Unexpected end of config. Expecting '") + end + "'");
			return res;
		}
		value = ParseConfig();

		if(value != NULL)
		{
			value->setParent(res);
			res->push(value);
		}

		SkipSpaces();
		c = seec();
		if(c == ',') getc();		// optional ','
	}

	return res;
}



double ConfigParser::getNumber(const char* body, int &start)
{
	int sign = 1;
	double res = 0.0, power = 1.0;
	if(body[start] == '-') sign = -1, start++;
	else
	if(body[start] == '+') sign = +1, start++;

	if(body[start]<'0' || body[start]>'9')
	{
		return 0.0f;
	}
	while(body[start]>='0' && body[start]<='9')
	{
		res = res*10 + (body[start] - '0');
		start++;
	}
	if(body[start]=='.')
	{
		start++;
		while(body[start]>='0' && body[start]<='9')
		{
			power *= 0.1;
			res = res + (body[start] - '0')*power;
			start++;
		}
	}
	return res*sign;
}

double ConfigParser::ParseNumber()
{
	int sign = 1;
	double res = 0.0, power = 1.0;
	char c = getc();
	if(c == '-') sign = -1, c =  getc();
	else
	if(c == '+') sign = +1, c =  getc();

	if(c < '0' || c > '9')
	{
		ungetc();
		LOG_PARSE_ERROR(string("Unexpected char '") + c + "'. Expecting digit");
		return 0.0f;
	}
	while(c >= '0' && c <= '9')
	{
		res = res*10 + (c - '0');
		c = getc();
	}
	if(c == '.')
	{
		c = getc();
		while(c >= '0' && c <= '9')
		{
			power *= 0.1;
			res = res + (c - '0')*power;
			c = getc();
		}
	}
	ungetc();
	return res*sign;
}

Config* ConfigParser::ParseNull()
{
	bool fail = false;
	fail = fail || getc() != 'n';
	fail = fail || getc() != 'u';
	fail = fail || getc() != 'l';
	fail = fail || getc() != 'l';
	if(fail)
	{
		LOG_PARSE_ERROR(string("Expecting null"));
	}
	return new ConfigNull;
}


std::string ConfigParser::ParseString()
{
	char c = getc();
	if(c != '\'' && c != '"')
	{
		LOG_PARSE_ERROR(string("Unexpected char '") + c + "'. Expecting ' or \"");
		ungetc();
		return string();
	}
	char end = c;
	std::string res;
	for(;;)
	{
		c = getc();
		if(c == end || c == '\0') break;
		if(c == '\\')
		{
			char d = seec();
			if(d == '\'' || d == '"' || d == '\\')
				c = getc();
			else
				LOG_PARSE_ERROR(string("Illegal escape sequence '\\") + d + "'. Accepting \\', \\\" or \\\\");
		}
		res.push_back(c);
	}
	
	if(c == '\0')
	{
		ungetc();
		LOG_PARSE_ERROR("Unexpected end of file in string");
		return res;
	}

	return res;
}

void ConfigParser::SkipSpaces()
{
	char c;
	char d;
/*
	state:
	0 - spaces
	1 - line comment (# //)
	2 - big comment  /* 
*/
	int state = 0;
	int level = 0;
	for(;;)
	{
		c = getc();
		switch(state)
		{
		case 0:
			switch(c)
			{
			case ' ':	case '\n':	case '\t':	case '\r': 
				break;
			case '#':
				state = 1;
				break;
			case '/':
				d = seec(); 
				if(d == '/'){ state = 1; getc(); }
				else if(d == '*') { state = 2; level = 1;  getc(); }
				else
				{
					ungetc();
					return;
				}
				break;
			default: // not space
				ungetc();
				return;
			}
			break;
		case 1: // #
			if(c == '\0') 
			{
				state = 0;
				ungetc();
			}
			if(c == '\n') state = 0;
			break;
		case 2: // /*
			if(c == '\0') 
			{
				LOG_PARSE_ERROR("Unexpected end of file in comment");
				state = 0;
				ungetc();
				break;
			}
			if(c == '/' && seec() == '*') { level++; break; }

			if(c == '*')
			{
				c = getc();
				if(c == '/')
				{
					level--;
					if(level == 0) state = 0;
				}
			}	
			break;
		}
	}
	ungetc();
}


bool ConfigParser::isFirstAlpha(char c)
{
	return (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' ||c == '_');
}

bool ConfigParser::isDigit(char c)
{
	return (c >= '0' && c <= '9');
}


char ConfigParser::seec(void)
{
	char c = file->getChar(position);
	return c;
}

char ConfigParser::getc(void)
{
	// TODO: UTF-8 line length

	char c = file->getChar(position);
	prevLine = line;
	prevCharNumber = charNumber;
	if(c == '\n')
	{
		line++;
		charNumber = 1;
	}
	else
		charNumber++;
	position++;
	return c;
}

void ConfigParser::ungetc(void)
{
	position--;
	line = prevLine;
	charNumber = prevCharNumber;
}

string ConfigParser::ParseError::getMessage(void)
{
	return message + " (" +  IntToStr(line) + ":" + IntToStr(charNumber) + ") [" + sender + "]";
}
