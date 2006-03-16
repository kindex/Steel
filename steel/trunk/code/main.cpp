
#include "graph/opengl_engine.h"
#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"
#include "common/logger.h"
#include <SDL.h>
#include "graph/primiteves/res_model.h"
#include "game/game.h"

int main(int argc, char *argv[])
{
	alog.open("steel.log");


	ResCollection res;

	res.registerClass(new BMP,	sizeof(BMP),	Res::image);
	res.registerClass(new _3DS, sizeof(_3DS),	Res::model);

	OpenGL_Engine graph;
//	graph.window.
	if(!graph.init()) return 1;


	Game game(&res, &graph);
	game.init();
	
	int cx = graph.window.width/2;
	int cy = graph.window.height/2;
	int sx = cx, sy = cy, mx, my;


	SDL_WarpMouse(cx, cy);

	int lastdx = 0, lastdy = 0;
	bool first = true;

	bool alive = true;
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

		game.process();

		SDL_Delay(1);
	}

	graph.deinit();
	alog.close();
	return 0;
}
