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
#include <math.h>

using namespace std;


Config*	ConfigParser::Parse(Text *_file)
{
	if(_file == NULL) return NULL;
	file = _file;
	position = 0;
	line = 1;
	charNumber = 1;
	Config *res = ParseConfig();

	SkipSpaces();
	char c = seec();
	
	if(c != '\0') 
		LOG_PARSE_ERROR(string("Unxpected symbol '") + c + "'. Expecting EOF");

	for(svector<ParseError>::iterator it = errors.begin(); it != errors.end(); it++)
	{
		log_msg("error parser", it->message + 
			" (" +  IntToStr(it->line) + ":" + IntToStr(it->charNumber) + ") [" +
			it->sender + "]");
	}


	return res;
}



Config* ConfigParser::ParseConfig()
{
	// TODO: parse tag
	return ParseValue();
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
	else
	{
		getc(); // skip char to avoid infinite cycle
		LOG_PARSE_ERROR(string("Illergal symbol '") + c + "'");
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


ConfigStruct* ConfigParser::ParseStruct()
{
	char c = getc();
	if(c != '{')
	{
		ungetc();
		LOG_PARSE_ERROR(string("Illergal symbol '") + c + "'");
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

		res->setValue(var, value);
		SkipSpaces();
		c = seec();
		if(c == ';' || c == ',') getc();		// optional ';' or ','
	}

	return res;
}


ConfigArray* ConfigParser::ParseArray()
{
	char c = seec();
	if(c != '(')
	{
		LOG_PARSE_ERROR(string("Illergal symbol '") + c + "'");
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
			res->push(value);

		SkipSpaces();
		c = seec();
		if(c == ';' || c == ',') getc();		// optional ';' or ','
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
			if(d == end || d == '\\') c = getc();
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
				else if(d == '*') { state = 2; getc(); }
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
			if(c == '\n') state = 0;
			break;
		case 2: // /*
			if(c == '*')
			{
				c = getc();
				if(c == '/') state = 0;
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
