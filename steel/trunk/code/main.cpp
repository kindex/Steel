/*id*********************************************************
	File: main.cpp
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [J. Anton, kane@mail.berlios.de]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		main() ????? ?????9? ?????9, ?????  ?} ????????, ??????? 
		??????9, ??????? ?????9?  ??? ???9.
 ************************************************************/

#include "steel.h"

#include "input/input.h"

#include "common/logger.h"
#include "common/timer.h"
#include "common/system_info.h"
#include "common/win_info.h"
#include "game/game.h"
#include "common/utils.h"
#include "res/res_main.h"
#include "main.h"
#include "graph/opengl/opengl_engine.h"

#include "audio/openal_engine.h"

using namespace std;

std::string commandLine;

#if (STEEL_COMPILER == COMPILER_GCC) && (STEEL_OS == OS_WIN32)
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	globalTimer.start(); globalFrameNumber = 1;

	deleteFiles("..\\log", "*.log");
	logFilter.set("-opengl_info");

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
		return 0;
	}

	Timer timer;
	timer.start();	timer.pause();

	float speed = 0.01f; // 100 FPS

// ******************* GRAPH ************************
	Input *input = new Input;

	OpenGL_Engine *graph = new OpenGL_Engine();
	if(!graph->init("opengl", input)) return 1;

// ******************* INPUT ************************
	if(!input->init("input")) return 1;
// ******************* GAME *************************
	Steel game;

	if(!game.init("game", input, commandLine)) return 1;

	game.bind(graph);


	AudioEngine *audio = new OpenALEngine();
	if (!audio->init("audio"))	return 1;

	audio->setConfig();
	audio->loadSources();

	game.bindAudioEngine(audio);


	//Listener *listener = new Listener();

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
				game.process(timer.total(), time);
			lastFrameTime = timer.total();
		}

		game.draw(graph);
		game.insonify(audio);

		timer.incframe();

		if(captionUdateTime + 0.5 < timer.total())
		{
			(graph->*(graph->setCaptionOpenGL_Window))(
				std::string("Sleel engine")
				+ " Obj: " + IntToStr(graph->total.object)
				+ " Trg: " + IntToStr(graph->total.triangle)
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
		globalFrameNumber++;
	}
	log_msg("core", "Exit from main loop");
// ******************* MAIN LOOP ************************
	(input->*input->FreeMouse)();

	graph->deinit();
	delete graph;

	destroyOpenAL();

	steel::log.close();
	return 0;
}
