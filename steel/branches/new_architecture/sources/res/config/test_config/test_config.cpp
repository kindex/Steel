/*id*********************************************************
	File: res/config/test_config/test_config.h
	Unit: Config
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Config Selftest
 ************************************************************/

#include "test_config.h"
#include "../../text/text_string.h"

using namespace std;


void ConfigParserTest::TestParser(void)
{
	TextString *file = new TextString();
	Config *config = NULL;

	ConfigParser parser;

// Test Base types parse correct
#define TEST_PARSER(config_text, expr, comment)								\
	TEST_CODE(file->Setup(config_text); config = parser.Parse(file));		\
	if(!parser.errors.empty())												\
	{	\
		for(svector<ConfigParser::ParseError>::iterator it = parser.errors.begin(); it != parser.errors.end(); it++)	\
			TEST_ERROR(string("Parse '") + file->getText() + "': " + it->getMessage());									\
	}\
	else	\
	{		\
		CHECK_NOT_NULL(config, string("Config '") + file->getText() + "' " + comment);	\
		if(config != NULL) CHECK_TRUE(expr, string("Config '") + file->getText() + "' " + comment);	\
	}\
	delete config; config = NULL;


// Test incorrect code
#define TEST_PARSER_INCORRECT(config_text, comment)								\
	TEST_CODE(file->Setup(config_text); config = parser.Parse(file));		\
	if(parser.errors.empty())												\
	{		\
			TEST_ERROR(string("'") + file->getText() + "': must not be parsed. " + comment);\
	}		\
	else	\
	{		\
/*		CHECK_TRUE(config == NULL, string("Config '") + file->getText() + "' " + comment);*/	\
	}\
	delete config; config = NULL;


// Test null
	TEST_PARSER("null", config->getType() == CONFIG_VALUE_NULL, "must be parsed to CONFIG_VALUE_NULL");
// Test comments
	TEST_PARSER("//Comment  \n123//Comment  ", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER (with comments)");
	TEST_PARSER("#Comment  \n123#Comment  ", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER (with comments)");
	TEST_PARSER("/*Comment  \n*/123/*Comment*/ ", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER (with comments)");
	TEST_PARSER("/*Co/*mm*/ent  \n*/123/*Comment*/ ", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER (with comments)");
	TEST_PARSER("/*C//omment  \n*/123/*Comment*/ ", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER (with comments)");

// Test correct numbers
	TEST_PARSER("123", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER");
	TEST_PARSER("-123", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == -123, "must be parsed to CONFIG_VALUE_NUMBER");
	TEST_PARSER("+123", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER");
	TEST_PARSER("\n \t+123\n \t", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123, "must be parsed to CONFIG_VALUE_NUMBER");
	TEST_PARSER("123.456", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123.456, "must be parsed to CONFIG_VALUE_NUMBER");
	TEST_PARSER("123.456e+10", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123.456e+10, "must be parsed to CONFIG_VALUE_NUMBER");
//	TEST_PARSER("\t\t  123.456e+10\n\n ", config->getType() == CONFIG_VALUE_NUMBER && config->returnd() == 123.456e+10, "must be parsed to CONFIG_VALUE_NUMBER");


// Test strings
	TEST_PARSER("'string'", config->getType() == CONFIG_VALUE_STRING && config->returns() == "string", "must be parsed to CONFIG_VALUE_STRING");
	TEST_PARSER("\"string\"", config->getType() == CONFIG_VALUE_STRING && config->returns() == "string", "must be parsed to CONFIG_VALUE_STRING");
	TEST_PARSER("   \"string\"   ", config->getType() == CONFIG_VALUE_STRING && config->returns() == "string", "must be parsed to CONFIG_VALUE_STRING");
	TEST_PARSER("   \"st\nr  ing\"   ", config->getType() == CONFIG_VALUE_STRING && config->returns() == "st\nr  ing", "must be parsed to CONFIG_VALUE_STRING");
	TEST_PARSER("\"st\\\"ring\"", config->getType() == CONFIG_VALUE_STRING && config->returns() == "st\"ring", "must be parsed to CONFIG_VALUE_STRING");
	TEST_PARSER("\"st\\\'ring\"", config->getType() == CONFIG_VALUE_STRING && config->returns() == "st'ring", "must be parsed to CONFIG_VALUE_STRING");
	TEST_PARSER("\"st\\\\ring\"", config->getType() == CONFIG_VALUE_STRING && config->returns() == "st\\ring", "must be parsed to CONFIG_VALUE_STRING");
// TODO: check UTF-8 string constants

// Struct
	TEST_PARSER("{}", config->getType() == CONFIG_VALUE_STRUCT, "must be parsed to CONFIG_VALUE_STRUCT");
	TEST_PARSER("{ x = 123 }", config->getType() == CONFIG_VALUE_STRUCT && config->find("x")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_STRUCT");
	TEST_PARSER("{ x = 1 y = 2}", config->getType() == CONFIG_VALUE_STRUCT && config->find("x")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_STRUCT");
	TEST_PARSER("{ x = 1; y = 2}", config->getType() == CONFIG_VALUE_STRUCT && config->find("x")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_STRUCT");
	TEST_PARSER("\n{\nx = 1\ny = 2\n}\n", config->getType() == CONFIG_VALUE_STRUCT && config->find("x")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_STRUCT");

// Array
	TEST_PARSER("( 123 )", config->getType() == CONFIG_VALUE_ARRAY && config->find("[0]")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_ARRAY");
	TEST_PARSER("( 1 2 )", config->getType() == CONFIG_VALUE_ARRAY && config->find("[0]")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_ARRAY");
	TEST_PARSER("( 1,2 )", config->getType() == CONFIG_VALUE_ARRAY && config->find("[0]")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_ARRAY");
	TEST_PARSER("\n(\n1\n2\n)\n", config->getType() == CONFIG_VALUE_ARRAY && config->find("[0]")->getType() == CONFIG_VALUE_NUMBER, "must be parsed to CONFIG_VALUE_ARRAY");

// Icorrect test examples
	TEST_PARSER_INCORRECT("value", "unquoted string constant");

// Test incorrect numbers
	TEST_PARSER_INCORRECT("123.4.5", "incorrect real number");
	TEST_PARSER_INCORRECT("--123", "incorrect real number");
	TEST_PARSER_INCORRECT("++123", "incorrect real number");
	TEST_PARSER_INCORRECT("+123.1e+12.3", "incorrect real number");
	TEST_PARSER_INCORRECT("123;", "incorrect real number");

// Test incorrect strings
	TEST_PARSER_INCORRECT("\"string", "unquoted string constant");
	TEST_PARSER_INCORRECT("\'string", "unquoted string constant");
	TEST_PARSER_INCORRECT("string'", "unquoted string constant");
	TEST_PARSER_INCORRECT("\"string\\\"", "unquoted string constant");
	TEST_PARSER_INCORRECT("\\", "unquoted string constant");
	TEST_PARSER_INCORRECT("'\\a'", "incorrect escape sequence");

// Test incorrect structs
	TEST_PARSER_INCORRECT("{", "struct");
	TEST_PARSER_INCORRECT("}", "struct");
	TEST_PARSER_INCORRECT("{)", "struct");
	TEST_PARSER_INCORRECT("{abc}", "illegal key name");
	TEST_PARSER_INCORRECT("{123}", "empty key name");
	TEST_PARSER_INCORRECT("{ = 123}", "empty key name");
	TEST_PARSER_INCORRECT("{ 123 = 124}", "illegal key name");
	TEST_PARSER_INCORRECT("{ abc = def}", "illegal value");
	TEST_PARSER_INCORRECT("{ abc = }", " empty value");
	TEST_PARSER_INCORRECT("{ x = 1, y = 2}", "; expected");
	TEST_PARSER_INCORRECT("{ x = 2;; }", "struct");
	TEST_PARSER_INCORRECT("{ x = 2,, }", "struct");
	TEST_PARSER_INCORRECT("{ x = 2 x = 2 }", "duplicate key");
	TEST_PARSER_INCORRECT("{ x = 2 x = 3 }", "duplicate key");
	TEST_PARSER_INCORRECT("{ פûגא = 2 }", "illegal key name");

// Test incorrect arrays
	TEST_PARSER_INCORRECT("(", "array");
	TEST_PARSER_INCORRECT(")", "array");
	TEST_PARSER_INCORRECT("(abc)", "array");
	TEST_PARSER_INCORRECT("(x=2)", "array");
	TEST_PARSER_INCORRECT("( = 123)", "array");
	TEST_PARSER_INCORRECT("(,)", "array");
	TEST_PARSER_INCORRECT("(;)", "array");
	TEST_PARSER_INCORRECT("(1 2;;3)", "array");
	TEST_PARSER_INCORRECT("( 1;2 )", ", expected");

// Test comments
	TEST_PARSER_INCORRECT("//123", "comment. no value");
	TEST_PARSER_INCORRECT("#123", "comment. no value");
	TEST_PARSER_INCORRECT("123 /*123", "comment with no end");
	TEST_PARSER_INCORRECT("123 */", "comment with no begin");

// Combined structs with result check
// TODO:

// Templates check
// TODO:


	#undef TEST_PARSER_INCORRECT
	#undef TEST_PARSER
}
