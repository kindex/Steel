
#include "_cpp.h"

#ifdef OPENGL_SDL	
	#include "graph/opengl/opengl_sdl_engine.h"
#endif
#ifdef OPENGL_WIN
	#include "graph/opengl/opengl_win_engine.h"
#endif


#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"
#include "res/material/material_conf.h"
#include "graph/primiteves/res_model.h"

#include "common/logger.h"
#include "common/timer.h"

#include "game/game.h"

#include <SDL.h>

int main(int argc, char *argv[])
{
	alog.open("steel.log");

	SDL_Init(SDL_INIT_TIMER);
	Timer_SDL timer;
	timer.start();
	double speed = 0.01; // 100 FPS

	ResCollection res;
	res.registerClass(new BMP,	sizeof(BMP),	Res::image);
	res.registerClass(new _3DS, sizeof(_3DS),	Res::model);
	res.registerClass(new MaterialConf, sizeof(MaterialConf),	Res::material);

#ifdef OPENGL_SDL	
	OpenGL_SDL_Engine graph;
#endif
#ifdef OPENGL_WIN
	OpenGL_WIN_Engine graph;
#endif

	graph.bindResColelntion(&res);
	if(!graph.init()) return 1;


	Game game(&res, &graph);
	game.init();
	
	int cx = graph.window.width/2;
	int cy = graph.window.height/2;
	int sx = cx, sy = cy, mx, my;

	SDL_WarpMouse(cx, cy);

	int lastdx = 0, lastdy = 0;


// ******************* MAIN LOOP ************************
	alog.msg("core", "Entering main loop");
	bool first = true, alive = true;
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
					if(!first && (event.motion.xrel != lastdx || event.motion.yrel != lastdy))
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
					first = false;
					break;
			}
		if(!alive)break;
	
		game.setspeed(speed, timer.total());
		game.process();
		timer.incframe();

		graph.setCaption(graph.window.title + " FPS = " + timer.getfps_s());
		speed = 1.0/timer.getfps();

		SDL_Delay(1);
	}
	alog.msg("core", "Exit from main loop");
// ******************* MAIN LOOP ************************

	graph.deinit();
	alog.close();
	return 0;
}
