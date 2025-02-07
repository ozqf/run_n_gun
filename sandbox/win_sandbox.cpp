#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <shellapi.h> // for parsing command line tokens

// read in pre-tokenised command line
extern int __argc;
extern char** __argv;

//#include <zengine.h>
#include <ze_sandbox.h>

extern "C" int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
	Sandbox_Run(__argc, __argv);
	return 0;
}
