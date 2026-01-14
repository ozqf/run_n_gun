#ifndef ZENGINE_INTERNAL_H
#define ZENGINE_INTERNAL_H

#include <zengine.h>

ze_external ZEApp       ZEGetApp();
ze_external void 		Platform_InitConsole();
ze_external zErrorCode  Platform_CreateWindow(const char* windowName);
ze_external void        Platform_PollEvents();
ze_external void        ZEPlatform_SaveImageRGB(const char *fileName, i32 width, i32 height, const void *rgbPixels);
ze_external zErrorCode  ZRGL_Init();

ze_external void ZE_CommandLineInit(i32 argc, char** argv);

#endif // ZENGINE_INTERNAL_H