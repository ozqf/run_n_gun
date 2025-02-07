
#include <ze_assets.h>

ze_internal bool g_running = YES;
ze_internal ZEApp g_app = {};

ze_external i32 ZEGetAppFlags() { return g_app.flags; }

ze_external ZEApp ZEGetApp() { return g_app; }

ze_external void *Platform_Alloc(zeSize size)
{
	return malloc(size);
}

ze_external void *Platform_Realloc(void* ptr, zeSize size)
{
	return realloc(ptr, size);
}

ze_external void Platform_Free(void *ptr)
{
	free(ptr);
}

ze_external void Platform_Shutdown()
{
	printf("Shutdown triggered\n");
	g_running = NO;
}

ze_external zErrorCode ZERunLoop(i32 targetFrameRate, ZE_FrameCallback callback)
{
	i32 g_targetFPS = 60;
	f64 g_targetDelta = 1.f / (f32)g_targetFPS;
	
	f64 lastTickTime = Platform_QueryClock();
	while (g_running)
	{
		f64 now = Platform_QueryClock();
		f64 delta = now - lastTickTime;
		// 	delta, now, lastTickTime);
		if (delta < g_targetDelta)
		{
			// if we have loads of time until the next frame, sleep
			if (g_targetDelta / delta > 2)
			{
				Platform_Sleep(1);
			}
			continue;
		}
		lastTickTime = now;
		// do
		Platform_PollEvents();
		ZEFrame frame = {};
		frame.delta = (f32)delta;
		callback(frame);
	}
	printf("Shutting down\n");
	return ZERROR_CODE_NONE;
}

////////////////////////////////////////////////////////
// initialisation
////////////////////////////////////////////////////////
/*ze_internal void Platform_InitConsole()
{
	if (g_bConsoleInit)
	{
		return;
	}
	g_bConsoleInit = TRUE;
	// init live debug console
	FILE *stream;
	AllocConsole();
	freopen_s(&stream, "conin$", "r", stdin);
	freopen_s(&stream, "conout$", "w", stdout);
	freopen_s(&stream, "conout$", "w", stderr);
	consoleHandle = GetConsoleWindow();
	MoveWindow(consoleHandle, 1, 1, 680, 600, 1);
	printf("[%s] Console initialized.\n", __FILE__);
}*/

ze_internal void DumpArgv(i32 argc, char** argv)
{
	if (argv == NULL)
	{
		return;
	}
	printf("Args (%d): ", argc);
	for (i32 i = 0; i < argc; ++i)
	{
		printf("%s,", argv[i]);
	}
	printf("\n");
}

ze_external zErrorCode ZE_EngineStart(ZEApp app, i32 argc, char** argv)
{
	Platform_InitConsole();
	DumpArgv(argc, argv);
	
	zErrorCode err = ZERROR_CODE_NONE;
	g_app = app;
	Platform_CreateWindow(app.windowName);
	err = ZRGL_Init();
	if (err != ZERROR_CODE_NONE)
	{
		return err;
	}
	
	return ZERROR_CODE_NONE;
}
