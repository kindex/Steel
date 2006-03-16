
#include "graph/opengl_engine.h"
#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"
#include "common/logger.h"
#include <SDL.h>
#include "graph/primiteves/res_model.h"

void test()
{
}

int main(int argc, char *argv[])
{
//	test();	return 0;
	ResCollection res;

	res.registerClass(new BMP, sizeof(BMP), Res::image);
	res.registerClass(new _3DS, sizeof(_3DS), Res::model);
	alog.open("steel.log");
	
	res.add(Res::image, "1");
	res.add(Res::model, "-");
	res.add(Res::model, "4");

	OpenGL_Engine graph;
	if(!graph.init()) return 1;

	graph.camera.seteye(v3(60, 50, 21));

	res_model world;

	world.assign((Model*)res["4"]);
	bool alive = true;
	while(alive)
	{
		SDL_Event event;
		if(SDL_PollEvent(&event))
			switch(event.type)
			{
				case SDL_QUIT:  
				case SDL_KEYDOWN:
					alive = false;
					break;
			}
		if(!alive)break;

		graph.clear();
		graph.inject(&world);
		graph.processCamera();
		graph.process();
		SDL_Delay(1);
	}

	graph.deinit();
	alog.close();
	return 0;
}
