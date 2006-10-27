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


void ConfigParserTest::TestNull(void)
{
	TextString *file = new TextString("null");
	Config *config = NULL;

	ConfigParser parser;
	config = parser.Parse(file);
	
	CHECK_NOT_NULL(config, string("Config '") + file->getText() + "' must parse to ConfigNull");
	CHECK_EQUAL(config->getType(), CONFIG_VALUE_NULL, string("Config '") + file->getText() + "' must parse to ConfigNull");

}
