#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <shellapi.h> // for parsing command line tokens
#include <stdio.h>
#include <stdlib.h>

#include <zengine.h>
#include <ze_sandbox.h>
/*
internal i32 g_bConsoleInit = FALSE;
internal HWND consoleHandle;

////////////////////////////////////////////////////////
// Console
////////////////////////////////////////////////////////
static void InitConsole()
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
}
*/
extern "C" int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
	zErrorCode err =  ZE_EngineInit();
	if (err != ZERROR_CODE_NONE)
	{
		printf("Engine startup failed with code %d\n", err);
		Sleep(2000);
		return 0;
	}
    Sandbox_Run();
	printf("Waiting 2 seconds\n");
	Sleep(4000);
	return 0;
}
