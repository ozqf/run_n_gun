#ifndef ZENGINE_INTERNAL_H
#define ZENGINE_INTERNAL_H

#include <zengine.h>

ze_external zErrorCode Platform_CreateWindow();
ze_external void Platform_SaveImage(const char *fileName, i32 width, i32 height, const void *rgbPixels);
ze_external zErrorCode ZRGL_Init();

#endif // ZENGINE_INTERNAL_H