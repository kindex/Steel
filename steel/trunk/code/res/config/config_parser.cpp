/*id*********************************************************
	File: res/config/config.h
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

#include "config.h"
#include <math.h>

using namespace std;

static int line, charn;

bool isFirstAlpha(char c)
{
	return (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' ||c == '_');
}

bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}


string ParseAlpha(const char* body, int &i)
{
	string res;
	char c = body[i];
	// skip spaces
	if(!isFirstAlpha(c))
	{
		parse_error("config parser", "Expecting character or _ at line " + IntToStr(line) + "[ALPHA]");
	}
	while(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ||c == '_')
	{
		res.push_back(c);
		c = body[++i];
	}
	if(res.empty())
	{
		i++;
		parse_error("config parser", "Empty AlphaString at line " + IntToStr(line) + "[ALPHA]");
	}

	return res;
}


ConfigStruct *ParseStruct(const char* body, int &i)
{
	if(body[i] != '{')
	{
		parse_error("config parser", string("Illergal symbol '") + body[i] + "' at line " + IntToStr(line) + "[STRUCT]");
		return NULL;
	}
	char end = '}';
	i++;

	ConfigStruct *res = new ConfigStruct();
	for(;;)
	{
		Config* value = NULL;

		SkipSpaces(body, i);
		if(body[i] == end)
		{
			i++;
			return res;
		}
		if(body[i] == '\0')
		{
			parse_error("config parser", string("Unexpected end of config. Expecting ") + end + " [STRUCT]");
			return res;
		}
		string var;
//		if(isFirstAlpha(body[i])) // var = value
//		{
			var = ParseAlpha(body, i);
			SkipSpaces(body, i);
			if(body[i] != '=') 
			{
				parse_error("config parser", string("Illegal symbol '") + body[i] + "' at line " + IntToStr(line) + ". Expecting = [STRUCT]");
				i++;
				continue;
			}
			i++;
			SkipSpaces(body, i);
//		}
#if 0
		else
		{
			if(isDigit(body[i])) // possible variants: int = config or simple number
			{
				double integer = ParseNumber(body, i);
/*				if(!integer)
				{
					parse_error("config parser", "Illergal number in struct at line " + IntToStr(line) + " [STRUCT]")
					continue;
				}*/
				SkipSpaces(body, i);
				if(body[i] == '=') // int = config
				{
					if(integer<0)
					{
						parse_error("config parser", "Array index must be > 0 at line " + IntToStr(line) + " [STRUCT]");
						continue;
					}
					if( fabs((double)(int)integer - integer) > EPSILON)
					{
						parse_error("config parser", "Array index must be integer at line " + IntToStr(line) + " [STRUCT]");
						continue;
					}
					var = IntToStr((int)integer);
					i++;
					SkipSpaces(body, i);
				}
				else
				{
					value = new ConfigNumber();
					((ConfigNumber*)value)->setValue(integer);
				}
			}
			if(var.empty())
				var = IntToStr(res->getFreeIndex());
		}
#endif
		if(!value)
			value = ParseConfig(body, i);

		res->setValue(var, value);
		SkipSpaces(body, i);
		if(body[i] == ';' || body[i] == ',') i++;		// optional ;
	}

	return res;
}


ConfigArray *ParseArray(const char* body, int &i)
{
	if(body[i] != '(')
	{
		parse_error("config parser", string("Illergal symbol '") + body[i] + "' at line " + IntToStr(line) + "[STRUCT]");
		return NULL;
	}
	char end = '}';
	i++;

	ConfigArray *res = new ConfigArray();
	for(;;)
	{
		Config* value = NULL;

		SkipSpaces(body, i);
		if(body[i] == end)
		{
			i++;
			return res;
		}
		if(body[i] == '\0')
		{
			parse_error("config parser", string("Unexpected end of config. Expecting ") + end + " [STRUCT]");
			return res;
		}

		value = ParseConfig(body, i);
		if(value != NULL)
			res->push(value);

		SkipSpaces(body, i);
		if(body[i] == ';' || body[i] == ',') i++;		// optional ;
	}

	return res;
}



double ParseNumber(const char* body, int &start)
{
	int sign = 1;
	double res = 0.0, power = 1.0;
	if(body[start] == '-') sign = -1, start++;
	else
	if(body[start] == '+') sign = +1, start++;

	if(body[start]<'0' || body[start]>'9')
	{
		parse_error("config parser", string("Illergal symbol '") + body[start] + "' at line " + IntToStr(line) + "[NUMBER]");
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


std::string ParseString(const char* body, int &start)
{
	int limit = body[start];
	std::string res;
	start++;

	while(body[start] != limit && body[start] != '\0')
	{
		res.push_back(body[start]);
		start++;
	}
	
	if(body[start] == '\0')
	{
		parse_error("config parser", "Unexpected end of string at line " + IntToStr(line) + "[STRING]");
		return res;
	}
	else
		start++;

	return res;
}

void SkipSpaces(const char* body, int &i)
{
	char c = body[i];
	// skip spaces
	// TODOL: skip comments
	while(c == ' ' || c == '\n' || c == '\t' || c == '\r')
	{
		if(c == '\n') line++, charn = 0;
		i++; charn++;
		c = body[i];
	}
}

Config *ParseConfig(const char* body, int &i)
{
	return ParseValue(body, i);
}

Config *ParseValue(const char* body, int &i)
{
	Config *res = NULL;

	SkipSpaces(body, i);
	if(body[i] == '\0') 
		return res;
		
	// Number
	if(body[i] == '+' || body[i] == '-' || body[i]>='0' && body[i] <= '9')
	{
		double number = ParseNumber(body, i);

		res = new ConfigNumber;
		((ConfigNumber*)res)->setValue(number);
	}else

	// String
	if(body[i] == '\'' || body[i] == '"')
	{
		res = new ConfigString;
		((ConfigString*)res)->setValue(ParseString(body, i));
	}else
	if(body[i] == '{') // Struct
	{
		res = ParseStruct(body, i);
	}
	else
	if(body[i] == '(') // Array
	{
		res = ParseArray(body, i);
	}
	else
	{
		parse_error("config parser", string("Illergal symbol '") + body[i] + "' at line " + IntToStr(line) + "[VALUE]");
		i++;
	}

/*	SkipSpaces(body, i);
	if(body[i] != '\0') 
		parse_error("config parser", string("Unxpected symbol '") + body[i] + "' at line " + IntToStr(line) + "[VALUE]");
*/
	return res;

}
