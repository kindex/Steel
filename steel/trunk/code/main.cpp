/*id*********************************************************
	Unit: core
	Part of: Steel engine
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

#include "_cpp.h"

#ifdef STEEL_OPENGL_SDL	
	#include "graph/opengl/opengl_sdl_engine.h"
	#include "input/input_sdl.h"
#endif
#ifdef STEEL_OPENGL_WIN
	#include "graph/opengl/opengl_win_engine.h"
	#include "input/input_win.h"
#endif

#include "common/logger.h"
#include "common/timer.h"

#include "game/game.h"
#include "common/utils.h"

#include "res/res_main.h"

using namespace std;

bool test();
std::string commandLine;

#ifdef STEEL_COMPILER_DEVCPP
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	globalTimer.start(); globalFrameNumber = 1;


	deleteFiles("..\\log", "*.log");
	//logFilter.set("error debug -res -script");

	steel::log.open("../steel.log");

	log_msg("core", "Command Line: '" + commandLine + "'");
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
#ifdef STEEL_OPENGL_SDL	
	OpenGL_SDL_Engine graph;
	InputSDL input;
	graph.bindResColelntion(&res);
	if(!graph.init("renderer")) return 1;
#endif
#ifdef STEEL_OPENGL_WIN
	OpenGL_WIN_Engine graph;
	InputWIN input;
	if(!graph.init("renderer", &input)) return 1;
#endif
	
// ******************* INPUT ************************
	if(!input.init("input")) return 1;
// ******************* GAME *************************
	Game game;

	if(!game.init("game", &input, commandLine)) return 1;

	game.bind(&graph);

// ******************* MAIN LOOP ************************
	steel::time captionUdateTime = -1;
	log_msg("core", "Entering main loop");
	bool first = true;
	steel::time	lastFrameTime = timer.total();

	while(input.isAlive() && game.isAlive())
	{
		input.process();
		double dx = 0, dy = 0;
		input.getMouseDelta(dx, dy);
		
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

		game.draw(&graph);

		timer.incframe();

		if(captionUdateTime + 0.5 < timer.total())
		{
			graph.setCaption(std::string("Sleel engine")
				+ " Obj: " + IntToStr(graph.total.object)
				+ " Trg: " + IntToStr(graph.total.triangle)
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
	input.freeMouse();

	graph.deinit();
	steel::log.close();
	return 0;
}
