
#include "graph/opengl_engine.h"
#include "res/res.h"
#include "res/image/bmp.h"
#include "res/model/_3ds.h"

void test()
{
}

int main()
{
//	test();	return 0;

	ResCollection a;

	a.registerClass(new BMP, sizeof(BMP), "bmp");
	a.registerClass(new _3DS, sizeof(_3DS), "3ds");

	a.add("1.bmp");
	a.add("4.3ds");


	OpenGL_Engine graph;
	if(!graph.init()) return 1;

//	GraphInterface world;

	MSG msg;
	while(true)											// Do our infinate loop
	{													// Check if there was a message
		BOOL wasmsg;
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

	graph.deinit();
	return 0;
}