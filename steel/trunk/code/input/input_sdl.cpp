#include "../_cpp.h"

#ifdef OPENGL_SDL

#include "input_sdl.h"
#include <SDL.h>
using namespace std;

void InputSDL::process()
{
		SDL_Event event;
		string key;
		if(SDL_PollEvent(&event))
			switch(event.type)
			{
				case SDL_QUIT:  
					alive = false;
					break;

				case SDL_KEYDOWN:
					key = SDL_GetKeyName(event.key.keysym.sym);
					keyPressed[key] = true;

					game->handleEventKeyDown(key);

					break;

				case SDL_KEYUP:
					key = SDL_GetKeyName(event.key.keysym.sym);
					keyPressed[key] = false;

					game->handleEventKeyUp(key);
					break;

				case SDL_MOUSEMOTION:
					if(!firstMouse && (event.motion.xrel != lastdx || event.motion.yrel != lastdy))
					{
						mx = event.motion.x;
						my = event.motion.y;

						SDL_WarpMouse(cx, cy);
				
						lastdx = cx - mx;
						lastdy = cy - my;

						game->handleMouse(lastdx, -lastdy);
					}
					else
					{
						lastdx = 0;
						lastdy = 0;
					}
					firstMouse = false;
					break;
	
			}
}

void InputSDL::getMouseDelta(int &dx, int &dy)
{

}


void InputSDL::captureMouse(int _cx, int _cy)
{
	mouseCaptured = true;

	cx = _cx;
	cy = _cy;
	sx = cx;
	sy = cy;

	SDL_WarpMouse(cx, cy);
}

#endif
