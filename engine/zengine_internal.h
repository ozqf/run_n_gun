#ifndef ZENGINE_INTERNAL_H
#define ZENGINE_INTERNAL_H

#include <zengine.h>

ze_external ZEApp       ZEGetApp();
ze_external zErrorCode  Platform_CreateWindow(const char* windowName);
ze_external void        Platform_PollEvents();
ze_external void        Platform_SaveImage(const char *fileName, i32 width, i32 height, const void *rgbPixels);
ze_external zErrorCode  ZRGL_Init();

#endif // ZENGINE_INTERNAL_H