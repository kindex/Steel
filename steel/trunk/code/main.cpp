/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
	Licence:
        “олько дл€ Division
    Description:
		main() создайт игровые классы, коллеццию ресурсов, загружет 
		плагины, содержит главный цикл игры.
 ************************************************************/

#include "_cpp.h"
#include "physic/physic_engine_3d.h"

#ifdef OPENGL_SDL	
	#include "graph/opengl/opengl_sdl_engine.h"
	#include "input/input_sdl.h"
#endif
#ifdef OPENGL_WIN
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

#ifdef COMPILER_DEVCPP
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	alog.open("../steel.log");
	alog.msg("core info", "Command Line: '" + commandLine + "'");

	if(!test())
	{
		alog.close();
		return 0;
	}

	Timer timer;
	timer.start();	timer.pause();

	float speed = 0.01f; // 100 FPS
// ******************** RES ************************
	ResCollection res;
	registerResources(res);

// ******************* GRAPH ************************
#ifdef OPENGL_SDL	
	OpenGL_SDL_Engine graph;
	InputSDL input;
	graph.bindResColelntion(&res);
	if(!graph.init("renderer")) return 1;
#endif
#ifdef OPENGL_WIN
	OpenGL_WIN_Engine graph;
	InputWIN input;
	graph.bindResColelntion(&res);
	if(!graph.init("renderer", &input)) return 1;
#endif
	
// ******************* INPUT ************************
	if(!input.init(&res, "input")) return 1;
// ******************* GAME *************************
	Game game;

	if(!game.init(&res, "game", &input, commandLine)) return 1;

	game.bind(&graph);

// ******************* MAIN LOOP ************************
	steel::time captionUdateTime = -1;
	alog.msg("core", "\n\t\tEntering main loop");
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

		if(captionUdateTime + 0.25 < timer.total())
		{
			graph.setCaption(std::string("Sleel engine")
				+ " Obj: " + IntToStr(graph.total.object)
				+ " Trg: " + IntToStr(graph.total.triangle)
				+ " Col: " + IntToStr(game.getCollisionCount())
				+ " FPS " + timer.getfps_s()

				);
			speed = 1.0f/timer.getfps();
			if(speed > 0.1f) speed = 0.1f;

			captionUdateTime = timer.total();
		}
		if(first)
		{
			first = false;
			alog.msg("core", "\n\t\tMain loop: first frame passed");
			timer.resume();
		}
	}
	alog.msg("core", "\n\t\tExit from main loop");
// ******************* MAIN LOOP ************************
	input.freeMouse();

	graph.deinit();
	alog.close();
	return 0;
}
