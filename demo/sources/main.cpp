/*id*********************************************************
	Unit: core
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		main() создайт игровые классы, коллеццию ресурсов, загружет 
		плагины, содержит главный цикл игры.
 ************************************************************/
#include <direct.h>

#include <steel.h>
#include <input/input.h>
#include <common/logger.h>
#include <common/timer.h>
#include <common/system_info.h>
#include <common/win_info.h>
#include <common/utils.h>
#include <res/res_main.h>
#include <graph/opengl/opengl_engine.h>
#include <audio/openal_engine.h>
#include <res/config/config_setup.h>

#include "main.h"
#include "demo.h"

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
	
	CollectSystemInfo();

	#if STEEL_OS == OS_WIN32
		DetectWindowsVersion();
	#endif

	// ******************** RES ************************
	registerResources();

	if(!test())
	{
		steel::log.close();
		return 99;
	}

	Timer timer;
	timer.start();	timer.pause();

	float speed = 0.01f; // 100 FPS

	Config* steelConfig = resConfig.add("../conf/demo.conf");
	if (steelConfig == NULL)
	{
		error("demo.conf", "Fatal error: conf/steel.conf was not found");
		return 1;
	}
	executeScript(steelConfig, commandLine);

// ******************* INPUT ************************
	Input* input = new Input;
	if(!input->init(steelConfig->find("input")))
	{
		error("demo.conf", "Fatal error: cannot find input config");
		return 2;
	}
// ******************* GRAPH ************************
	OpenGL_Engine* graph = new OpenGL_Engine();
	if(!graph->init(steelConfig->find("graph"), input))
	{
		error("demo.conf", "Fatal error: cannot find graph config");
		return 3;
	}
// ******************* AUDIO ************************
	AudioEngine *audio = NULL;
#ifdef LIB_OPENAL
	audio = new OpenALEngine();
#endif
	if(audio != NULL && !audio->init(steelConfig->find("audio")))
	{
		error("demo.conf", "Warning: cannot find audio config");
		delete audio;
		audio = NULL;
	}

// ******************* GAME *************************
	Demo game;

	if(!game.init(steelConfig, input)) return 5;


	game.bind(graph);
	if(audio)
	{
		game.bind(audio);
		game.insonify(audio);
	}
		

// ******************* MAIN LOOP ************************
	steel::time captionUdateTime = -1;
	log_msg("core", "Entering main loop");
	
	bool first = true;
	steel::time	lastFrameTime = timer.total();

	while(input->isAlive() && game.isAlive())
	{
		(input->*(input->Process))();
		double dx = 0, dy = 0;
		input->getMouseDelta(dx, dy);
		
		game.handleMouse(dx, -dy);

		if(speed < 0.0 || speed>0.01 && timer.total()<2)
		{
			speed = 0.01f;
		}
	
		game.setspeed(speed, timer.total());
	
		if(!first)
		{
			steel::time time = timer.total() - lastFrameTime;
			if(time>0)
			{
				game.process(timer.total(), time);
			}
			lastFrameTime = timer.total();
		}

		game.draw(graph);
		if(audio) game.insonify(audio);

		timer.incframe();

		if(captionUdateTime + 0.5 < timer.total())
		{
			(graph->*(graph->setCaptionOpenGL_Window))(
				std::string("Sleel engine")
				+ " Obj: " + IntToStr(graph->total.objectCount)
				+ " Trg: " + IntToStr(graph->total.triangleCount)
//				+ " Col: " + IntToStr(game.getCollisionCount())
				+ " FPS " + timer.getfps_s()
			);

			speed = 1.0f/timer.getfps();
			if(speed > 0.1f) speed = 0.1f;

			captionUdateTime = timer.total();
		}
		if(first)
		{
			first = false;
			log_msg("core", "Main loop: first frame passed");
			timer.resume();
		}
//		globalFrameNumber++;
	}
	log_msg("core", "Exit from main loop");
// ******************* MAIN LOOP ************************
	(input->*input->FreeMouse)();

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
