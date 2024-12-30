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
	ZE_EngineInit();
    rng_run();
	Sleep(2000);
	return 0;
}
