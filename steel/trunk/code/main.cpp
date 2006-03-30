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
#endif
#ifdef OPENGL_WIN
	#include "graph/opengl/opengl_win_engine.h"
#endif

#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"
#include "res/conf/conf_text.h"

#include "common/logger.h"
#include "common/timer.h"

#include "game/game.h"
#include "utils.h"

#include <SDL.h>

Res* createBMP()		{return new BMP; }
Res* create3DS()		{return new _3DS; }
Res* createConfig()		{return new ConfigText; }
Res* createNormalMap()	{return new NormalMap; }
Res* createHeightMap()	{return new HeightMap; }

int main(int argc, char *argv[])
{
	alog.open("steel.log");

	SDL_Init(SDL_INIT_TIMER);
	Timer_SDL timer;
	timer.start();	timer.pause();

	double speed = 0.01; // 100 FPS
// ************* RES ****************
	ResCollection res;
	res.registerClass(createBMP,	Res::image);
	res.registerClass(create3DS,	Res::model);
	res.registerClass(createConfig,	Res::config);
	res.registerClass(createNormalMap, Res::normalMap);
	res.registerClass(createHeightMap, Res::normalMap);

// *************** GRAPH *******************
#ifdef OPENGL_SDL	
	OpenGL_SDL_Engine graph;
#endif
#ifdef OPENGL_WIN
	OpenGL_WIN_Engine graph;
#endif

	graph.bindResColelntion(&res);

	if(!graph.init("renderer")) return 1;
	
	Game game(&res);
	game.init();
	
	int cx = graph.conf->geti("width")/2;
	int cy = graph.conf->geti("height")/2;
	int sx = cx, sy = cy, mx, my;

	SDL_WarpMouse(cx, cy);

	int lastdx = 0, lastdy = 0;
// ******************* PHYSIC **************************

	PhysicEngine physic;
	physic.bindResColelntion(&res);
	if(!physic.init("physic")) return 1;

// ******************* MAIN LOOP ************************
	time captionUdateTime = -1;
	alog.msg("core", "Entering main loop");
	bool first = true, firstMouse = true, alive = true;
	while(alive && game.alive())
	{
		SDL_Event event;
		if(SDL_PollEvent(&event))
			switch(event.type)
			{
				case SDL_QUIT:  
					alive = false;
					break;

				case SDL_KEYDOWN:
					game.handleEventKeyDown(SDL_GetKeyName(event.key.keysym.sym));
					break;

				case SDL_KEYUP:
					game.handleEventKeyUp(SDL_GetKeyName(event.key.keysym.sym));
					break;

				case SDL_MOUSEMOTION:
					if(!firstMouse && (event.motion.xrel != lastdx || event.motion.yrel != lastdy))
					{
						mx = event.motion.x;
						my = event.motion.y;

						SDL_WarpMouse(cx, cy);
				
						lastdx = cx - mx;
						lastdy = cy - my;

						game.handleMouse(lastdx, -lastdy);
					}
					else
					{
						lastdx = 0;
						lastdy = 0;
					}
					firstMouse = false;
					break;
			}
		if(!alive)break;

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

		if(captionUdateTime+1<timer.total())
		{
			graph.setCaption(std::string("Sleel engine")
				+ " FPS = " + timer.getfps_s()
				+ " Obj: " + IntToStr(graph.total.object)
				+ " Trg: " + IntToStr(graph.total.triangle)
				
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

		SDL_Delay(1);
	}
	alog.msg("core", "Exit from main loop");
// ******************* MAIN LOOP ************************

	graph.deinit();
	alog.close();
	return 0;
}
