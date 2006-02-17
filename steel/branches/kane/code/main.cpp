
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

#ifdef __linux
	while ( true )
	{
		SDL_Event event;
		if ( SDL_PollEvent(&event) )
			if ( event.type==SDL_QUIT ) break;
		graph.clear();
		graph.process();
		SDL_Delay(1);
	}
#else	// __linux
	MSG msg;
	while(true)											// Do our infinate loop
	{													// Check if there was a message
		bool wasmsg;
        if (wasmsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)					// If the message wasnt to quit
		        break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
		}
		if(!wasmsg || graph.window->needupdate)											// if there wasn't a message
		{
			graph.clear();
//			graph.inject(world);

			graph.process();								// Render the scene every frame
			Sleep(1);
		}
	}
	return(msg.wParam);									// Return from the program
#endif	// __linux

	graph.deinit();
	alog.close();
	return 0;
}
