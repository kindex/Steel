/*id*******************************************************************************
	File: res/font/font_text.h
	Unit: res/font
	Part of: Steel engine
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
        BMFont text file support
**************************************************************************************/

#include "../../steel.h"
#include "font_text.h"
#include "../res_main.h"
#include "../rstream.h"
#include "../../common/logger.h"
#include "../../common/containers/string_dict.h"
#include "../../common/cast.h"

using namespace std;

static void parseString(const std::string& line, 
                        OUT   std::string& kind,
                        OUT   StringDict& params)
{
    string::size_type p = line.find(' ');
    if (p == string::npos)
    {
        return;
    }
    kind = line.substr(0, p);
    do
    {
        string::size_type a = p;
        p = line.find('=', p + 1);
        string key = line.substr(a+1, p == string::npos ? string::npos : p-a-1); 
        a = p;
        if (p != string::npos)
        {
            string value;
            if (line[p+1] == '"')
            {
                p = line.find('"', p + 2);
                value = line.substr(a + 2, p == string::npos ? string::npos : p-a-2);
                if (p != string::npos)
                {
                    p++;
                }
            }
            else
            {
                p = line.find(' ', p + 1);
                value = line.substr(a + 1, p == string::npos ? string::npos : p-a-1);
            }
            params[trim(key)] = trim(value);
        }
    }
    while (p != std::string::npos);
}

bool FontText::init(const std::string& name, const std::string dir)
{
	std::string file = dir + "/" + name;

	rstream f(file, "fnt"); // no binary acces, only text
	if (f.fail()) 
	{
		log_msg("res conf error", "Cannot open file " + file);
		return false;
	}

    while (f.good())
    {
        std::string line;
        std::getline(f, line);
        std::string kind;
        StringDict params;
        parseString(line, kind, params);
        if (kind == "info")
        {
            size = cast<int>(params["size"]);
        }
        else if (kind == "page" && texture == NULL)
        {
            texture = resImage.add(params["file"]);
            if (texture == NULL)
            {
                return false;
            }
        }
        else if (kind == "char")
        {
            int id = cast<int>(params["id"]);
            Char c;
            c.x = cast<int>(params["x"]);
            c.y = cast<int>(params["y"]);
            c.width = cast<int>(params["width"]);
            c.height = cast<int>(params["height"]);
            c.xoffset = cast<int>(params["xoffset"]);
            c.yoffset = cast<int>(params["yoffset"]);
            c.xadvance = cast<int>(params["xadvance"]);
            chars[id] = c;
        }
    }
    default_char = chars['*'];

    return true;
}
