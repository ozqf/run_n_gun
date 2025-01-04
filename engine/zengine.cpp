#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <shellapi.h> // for parsing command line tokens
#include <stdio.h>
#include <stdlib.h>

#include <zengine_internal.h>

internal i32 g_bConsoleInit = FALSE;
internal HWND consoleHandle;

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

////////////////////////////////////////////////////////
// initialisation
////////////////////////////////////////////////////////
static void Platform_InitConsole()
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

extern "C" zErrorCode ZE_EngineInit()
{
	zErrorCode err = ZERROR_CODE_NONE;
	Platform_InitConsole();
	Platform_CreateWindow();
	err = ZRGL_Init();
	if (err != ZERROR_CODE_NONE)
	{
		return err;
	}
	
	return ZERROR_CODE_NONE;
}
