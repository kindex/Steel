
#include "graph/opengl_engine.h"
#include "res/res.h"
#include "res/image/bmp.h"
#include "common/logger.h"
#include "SDL.h"

void test()
{
}

int main()
{
//	test();	return 0;

	alog.open("steel.log");
	
	ResCollection a;

	a.registerClass(new BMP, sizeof(BMP), "bmp");

	a.add("1.bmp");
	a.add("2.bmp");
	a.add("3.bmp");
	a.add("3.bmp");
	a.add("3.bmp");


	OpenGL_Engine graph;
	if(!graph.init()) return 1;

//	GraphInterface world;

	while ( true )
	{
		SDL_Event event;
		if ( SDL_PollEvent(&event) )
			if ( event.type==SDL_QUIT ) break;
		graph.clear();
		graph.process();
		SDL_Delay(1);
	}

	graph.deinit();
	alog.close();
	return 0;
}
