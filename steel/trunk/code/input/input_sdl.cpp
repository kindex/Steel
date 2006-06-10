#include "../_cpp.h"

#ifdef OPENGL_SDL

#include "input_sdl.h"
#include <SDL.h>

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
	if(mouseCaptured && focused)
	{
		SDL_WarpMouse(cx, cy);
	}
}


void InputSDL::captureMouse()
{
	mouseCaptured = true;

	lastdx = 0;
	lastdy = 0;

	SDL_WarpMouse(cx, cy);
}

void InputSDL::freeMouse()
{
//	ShowCursor(true);			// show Mouse Pointer
	mouseCaptured = false;
}


#endif
