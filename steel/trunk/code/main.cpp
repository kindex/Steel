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

#include "physic/physic_engine.h"

#ifdef OPENGL_SDL	
	#include "graph/opengl/opengl_sdl_engine.h"
	#include "input/input_sdl.h"
#endif
#ifdef OPENGL_WIN
	#include "graph/opengl/opengl_win_engine.h"
	#include "input/input_win.h"
#endif

#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"
#include "res/conf/conf_text.h"

#include "common/logger.h"
#include "common/timer.h"

#include "game/game.h"
#include "utils.h"

//#include <SDL.h>

Res* createBMP()		{return new BMP; }
Res* create3DS()		{return new _3DS; }
Res* createConfig()		{return new ConfigText; }

#ifdef COMPILER_DEVCPP
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	alog.open("steel.log");

	Timer_SDL timer;
	timer.start();	timer.pause();

	double speed = 0.01; // 100 FPS
// *************** RES *****************
	ResCollection res;
	res.registerClass(createBMP,	Res::image);
	res.registerClass(create3DS,	Res::model);
	res.registerClass(createConfig,	Res::config);

// *************** GRAPH *****************
#ifdef OPENGL_SDL	
	OpenGL_SDL_Engine graph;
#endif
#ifdef OPENGL_WIN
	OpenGL_WIN_Engine graph;
#endif

	graph.bindResColelntion(&res);

	if(!graph.init("renderer")) return 1;
	
// ************ GAME **************
	Game game(&res);
	game.init();

// ************ INPUT ************
#ifdef OPENGL_SDL	
	InputSDL input;
#endif
#ifdef OPENGL_WIN
	InputWIN input;
#endif

	if(!input.init("", &game)) return 1;
	input.captureMouse(graph.conf->geti("width")/2, graph.conf->geti("height")/2);

// ******************* PHYSIC **************************

	PhysicEngine physic;
	physic.bindResColelntion(&res);
	if(!physic.init("physic")) return 1;

// ******************* MAIN LOOP ************************
	steel::time captionUdateTime = -1;
	alog.msg("core", "Entering main loop");
	bool first = true;
	while(input.isAlive() && game.isAlive())
	{
		input.process();
		int dx = 0, dy = 0;
		input.getMouseDelta(dx, dy);
		
		game.handleMouse(dx, -dy);

		if(speed < 0.0 || speed>0.01 && timer.total()<2)
		{
			speed = 0.01;
		}
	
		game.setspeed(speed, timer.total());
	
		physic.clear();
		game.processPhysic(&physic);

		game.process();

		graph.clear();
		game.draw(&graph);

		timer.incframe();

		if(captionUdateTime + 0.25 < timer.total())
		{
			graph.setCaption(std::string("Sleel engine")
				+ " FPS = " + timer.getfps_s()
				+ " Obj: " + IntToStr(graph.total.object)
				+ " Trg: " + IntToStr(graph.total.triangle)
				+ " Time: " + FloatToStr(timer.total())
				);
			speed = 1.0/timer.getfps();

			captionUdateTime = timer.total();
		}

		if(first)
		{
			first = false;
			alog.msg("core", "Main loop: first frame passed");
			timer.resume();
		}

//		SDL_Delay(1);
	}
	alog.msg("core", "Exit from main loop");
// ******************* MAIN LOOP ************************

	graph.deinit();
	alog.close();
	return 0;
}
