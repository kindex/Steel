
#include "graph/opengl_engine.h"
#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"
#include "common/logger.h"
#include <SDL.h>
#include "graph/primiteves/res_model.h"

ResCollection res;

void test()
{
}

int main(int argc, char *argv[])
{
//	test();	return 0;

	res.registerClass(new BMP, sizeof(BMP), "bmp");
	res.registerClass(new _3DS, sizeof(_3DS), "3ds");
	alog.open("steel.log");
	
	ResCollection a;

	res.add("1.bmp");
	res.add("-.3ds");
	res.add("4.3ds");


	OpenGL_Engine graph;
	if(!graph.init()) return 1;

	res_model world;

	world.assign((Model*)res["-.3ds"]);
	while ( true )
	{
		SDL_Event event;
		if ( SDL_PollEvent(&event) )
			if ( event.type==SDL_QUIT ) break;
		graph.clear();
		graph.inject(&world);
		graph.process();
		SDL_Delay(1);
	}

	graph.deinit();
	alog.close();
	return 0;
}
