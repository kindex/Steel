/*id*********************************************************
	File: objects/console.cpp
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Консоль для вывода отладочной информации на экран в графическом режиме
 ************************************************************/
#include "../steel.h"
#include "console.h"
#include "../common/logger.h"
#include "../res/res_main.h"

Console::Console() :
    line_count(0)
{}

bool Console::updateInformation(Engine& engine, IN const InterfaceId interface_id)
{
	if (interface_id == INTERFACE_GRAPH)
	{
        GraphInterface& gengine = *dynamic_cast<GraphInterface*>(&engine);

        GraphTextVector tv;
        int line = 0;
        float height = 0.06f;
        for (std::deque<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
        {
            GraphText text(*it, 
                           ObjectPosition::createTranslationMatrix(v3(-1, 1 - line*height, 0)),
                           POSITION_SCREEN,
                           v2(height, height),
                           SPRITE_ALIGN_SCREEN,
                           GraphText::ALIGN_LEFT_TOP,
                           resFont.add("/font/arial"),
                           color4f(1, 1, 1));
            tv.push_back(text);
            line++;
        }
        gengine.setGraphText(tv);

		return true;
	}
	return false;
}

void Console::write(const std::string& line)
{
    while (line_count > 10)
    {
        shift();
    }
    lines.push_back(line);
    line_count++;
    log_msg("console", line);
}

void Console::shift()
{
    line_count--;
    lines.pop_front();
}

void Console::clear()
{
    lines.clear();
}

Console console;
