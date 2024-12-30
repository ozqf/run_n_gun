#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <shellapi.h> // for parsing command line tokens
#include <stdio.h>
#include <stdlib.h>

#include <zengine.h>

internal i32 g_bConsoleInit = FALSE;
internal HWND consoleHandle;

extern "C" int main(
    int argc, char** argv)
{
	printf("ZEALOUS ENGINE CLI TOOLS");
	return 0;
}
