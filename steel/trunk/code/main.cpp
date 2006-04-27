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

#include "res/res.h"
#include "res/image/bmp.h"
#include "res/image/tga.h"
#include "res/model/_3ds.h"
#include "res/conf/conf_text.h"
#include "res/script/script_text.h"

#include "common/logger.h"
#include "common/timer.h"

#include "game/game.h"
#include "common/utils.h"

//#include <SDL.h>


// Image filename:
// name[.cube][.normal|.height][.bmp|.tga]

Res *createBitmap(const std::string filename, ResCollection *res)
{
	Res *r;
	// try to load image2d
	if(r = createBMP(filename, res)) return r;
	if(r = createTGA(filename, res)) return r;
	return NULL;
}

Res *createImageFormat(const std::string filename, ResCollection *res)
{
	Res *r;
	// try to load image2d
	if(r = createBitmap(filename, res))	return r;
	if(r = createBitmap(filename + ".normal", res))	
	{
		((Image*)r)->setFormat(ImageFormat::normal);
		return r;
	}
	if(r = createBitmap(filename + ".height", res))
	{
		((Image*)r)->convertFromHeightMapToNormalMap();
		((Image*)r)->setFormat(ImageFormat::normal);
		return r;
	}

	return NULL;
}


Res *createImage(const std::string filename, ResCollection *res)
{
	Res *r;
	// try to load image2d
	if(r = createImageFormat(filename, res)) return r;
	// try to load cubemap
	if(r = createImageFormat(filename + ".cube", res))
	{
		((Image*)r)->setKind(ImageKind::cube);
		return r;
	}
	return NULL;
}


#ifdef COMPILER_DEVCPP
int main1(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	alog.open("steel.log");

	Timer timer;
	timer.start();	timer.pause();

	double speed = 0.01; // 100 FPS
// *************** RES *****************
	ResCollection res;
	res.registerClass(createImage,		Res::image);
	res.registerClass(create3DS,		Res::model);
	res.registerClass(createMaterial,	Res::material);
	res.registerClass(createConfigText,	Res::config);
	res.registerClass(createScriptText,	Res::script);


// *************** GRAPH *****************
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
	
// ************ INPUT ************
	if(!input.init(&res, "input")) return 1;
// ************ GAME **************
	Game game;
	if(!game.init(&res, "game", &input)) return 1;

	game.bind(&graph);

// ******************* PHYSIC **************************

	PhysicEngine3D physic;
	physic.bindResColelntion(&res);
	if(!physic.init("physic")) return 1;

// ******************* MAIN LOOP ************************
	steel::time captionUdateTime = -1;
	alog.msg("core", "\n\t\tEntering main loop");
	bool first = true;
	while(input.isAlive() && game.isAlive())
	{
		input.process();
		double dx = 0, dy = 0;
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
			alog.msg("core", "\n\t\tMain loop: first frame passed");
			timer.resume();
		}

//		SDL_Delay(1);
	}
	alog.msg("core", "\n\t\tExit from main loop");
// ******************* MAIN LOOP ************************
	input.freeMouse();

	graph.deinit();
	alog.close();
	return 0;
}

