/*
Windows entry point for Run N Gun
*/
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <win_zengine.h>
#include <run_n_gun.h>

extern "C" int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    rng_start(__argc, __argv);
	return 0;
}
