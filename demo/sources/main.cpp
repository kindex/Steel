/*id*********************************************************
	Unit: core
	Part of: DiVision intro
	(C) DiVision, 2004-2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		main() создайт игровые классы, коллеццию ресурсов, загружет 
		плагины, содержит главный цикл игры.
 ************************************************************/
#include <steel.h>
#include <common/logger.h>
#include <common/timer.h>
#include <common/system_info.h>
#include <common/win_info.h>
#include <common/utils.h>
#include <res/res_main.h>
#include <graph/opengl/opengl_engine.h>
#include <audio/openal_engine.h>
#include <res/config/config_parser.h>
#include <objects/console.h>
#include "main.h"
#include "game_factory.h"
#include <input/input_win.h>
#include <direct.h>

/*
If you cannot find file svn.inc, just install python from http://python.org/
svn.inc generates automatically from SVN revision number and date.
It contains two constants like:
static const unsigned int SVN_REVISION = XXX;
static const char SVN_DATE[] = "DATE";
*/
#include "svn.inc"

using namespace std;

std::string commandLine;

#if (STEEL_COMPILER == COMPILER_GCC) && (STEEL_OS == OS_WIN32)
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
// *********************** LOG *********************
	globalTimer.start();

	deleteFiles("..\\log", "*.log");
	_mkdir("..\\log");
	logFilter.set("+opengl_info +system_info +windows_info");
	steel::log.open("../steel.log");
	log_msg("core", "Command Line: '" + commandLine + "'");
	log_msg("core", "Steel Engine revision: " + IntToStr(SVN_REVISION) + " " + SVN_DATE);
	CollectSystemInfo();

	#if STEEL_OS == OS_WIN32
		DetectWindowsVersion();
	#endif

	// ******************** RES ************************
	addDataSearchDirectories("../data/");
	addDataSearchDirectories("../../../steel.res/demo/data/"); // is needed only in developer mode
	registerResources();

	if (!test())
	{
		steel::log.close();
		error("core test", "Tests failed");
		return 99;
	}

	float speed = 0.01f; // 100 FPS

	if (commandLine.empty())
	{
		commandLine = "[../conf/demo.conf#] {}";
	}
	Config* steelConfig = parseConfig(commandLine);

	if (steelConfig == NULL)
	{
		error("core config fatal", "Cannot parse config : " + commandLine);
		return 1;
	}

// ******************* INPUT ************************
	InputWin* input = new InputWin;
	if (!input->init(steelConfig->find("input")))
	{
		error("core config fatal", "Cannot initialize input engine");
		return 2;
	}
// ******************* GRAPH ************************
	opengl::OpenGL_Engine* graph = new opengl::OpenGL_Engine();
	if (!graph->init(steelConfig->find("graph"), input))
	{
		error("core config fatal", "Cannot initialize graph engine");
		return 3;
	}
// ******************* AUDIO ************************
	AudioEngine *audio = NULL;
	Config* audioConfig = steelConfig->find("audio");
	if (audioConfig == NULL)
    {
		error("core config", "Cannot find audio config");
    }
    else
    {
        if (!audioConfig->getb("enabled", true))
        {
            warn("core config", "Audio engine is disabled in config");
        }
        else
        {
            #ifdef LIB_OPENAL
	            audio = new OpenALEngine();
            #endif
            if (audio == NULL)
            {
        		error("core config", "No audio support");
            }
            else
            {
                if (!audio->init(*audioConfig))
	            {
		            error("core config", "Cannot initialize audio engine");
		            delete audio;
		            audio = NULL;
	            }
            }
        }
    }
// ******************* GAME *************************
	GameFactory gameFactory;
	Game* game = gameFactory.createGame(steelConfig->gets("game_class"));
	if (game == NULL)
	{
		error("core config fatal", "Unknown game class '" + steelConfig->gets("game_class") + "'");
		return 5;
	}

	if (!game->init(*steelConfig, *input))
	{
		return 6;
	}

	game->bind(*graph);
	if (audio != NULL)
	{
		game->bind(*audio);
		game->insonify(*audio); // TODO: ?
	}

// ******************* MAIN LOOP ************************
	steel::time captionUdateTime = -1;
	log_msg("core", "Entering main loop");

	game->start();

	bool first = true;

	while (input->isAlive() && game->isAlive())
	{
        input->Process_WinAPI();
		double dx = 0, dy = 0;
		input->getMouseDelta(dx, dy);
		
		game->handleMouse(dx, -dy);
		game->process();
        console.process();

		game->draw(*graph);
		if (audio != NULL)
		{
			game->insonify(*audio);  // update listener coords
		}
        graph->setCaption(game->getWindowCaption());

		if (first)
		{
			first = false;
			log_msg("core", "Main loop: first frame passed");
		}
	}
	log_msg("core", "Exit from main loop");
// ******************* MAIN LOOP ************************
    delete game;

	input->FreeMouse_WinAPI();

	graph->deinit();
	delete graph;

/*	if(audio) 
	{
		audio->deinit();
		delete audio;
	}
*/
	steel::log.close();
	return 0;
}

#if STEEL_OS == OS_WIN32

#if STEEL_COMPILER == COMPILER_GCC
HINSTANCE hInstance;

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	::hInstance = hInstance;

	commandLine = lpCmdLine;

	return main1(0, NULL);							// Exit The Program
}

#elif (STEEL_COMPILER == COMPILER_VS8) && (STEEL_OPENGL_API == OPENGL_WINAPI)
HINSTANCE hInstance;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	::hInstance = hInstance;

	commandLine = lpCmdLine;

	return main(0, NULL);
}
#endif	// STEEL_COMPILER && STEEL_OPENGL_API

#endif	// STEEL_OS
