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

const int CONSOLE_LINE_COUNT = 10;
const int CONSOLE_VISIBLE_TIME = 10;

#include "../steel.h"
#include "console.h"
#include "../common/logger.h"
#include "../res/res_main.h"

Console::Console() :
    line_count(0)
{
    timer.start();    
}

bool Console::updateInformation(Engine& engine, IN const InterfaceId interface_id)
{
	if (interface_id == INTERFACE_GRAPH)
	{
        GraphInterface& gengine = *dynamic_cast<GraphInterface*>(&engine);

        GraphTextVector tv;
        int line = 0;
        float height = 0.06f;
        for (std::deque<Line>::iterator it = lines.begin(); it != lines.end(); it++)
        {
            GraphText text(it->message, 
                           ObjectPosition::createTranslationMatrix(v3(-1, 1 - line*height, 0)),
                           POSITION_SCREEN,
                           v2(height, height),
                           SPRITE_ALIGN_SCREEN,
                           GraphText::ALIGN_LEFT_TOP,
                           resFont.add("/font/arial"),
                           it->color);
            tv.push_back(text);
            line++;
        }
        gengine.setGraphText(tv);

		return true;
	}
	return false;
}

void Console::write(const std::string& message, const color4f color)
{
    while (line_count > CONSOLE_LINE_COUNT)
    {
        shift();
    }
    lines.push_back(Line(message, color, timer.total()));
    line_count++;
    log_msg("console", message);
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

Console::Line::Line(const std::string& message, const color4f color, const steel::time timestamp) :
    message(message),
    color(color),
    timestamp(timestamp)
{}

void Console::process()
{
    steel::time current = timer.total();
    while (!lines.empty() && lines.front().timestamp + CONSOLE_VISIBLE_TIME < current)
    {
        shift();
    }
}


Console console;
