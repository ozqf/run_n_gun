#ifndef ZE_ASSETS_H
#define ZE_ASSETS_H

#include <zengine_internal.h>

#define STBI_NO_STDIO

//#define STBI_MALLOC(sz)           malloc(sz)
//#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
//#define STBI_FREE(p)              free(p)

#define STBI_MALLOC(sz)			Platform_Alloc(sz)
#define STBI_REALLOC(p,newsz)	Platform_Realloc(p, newsz)
#define STBI_FREE(p)			Platform_Free(p)

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__

#include <stb_image_write.h>

ze_external void ZEPlatform_SaveImageRGB(
	const char *fileName, i32 width, i32 height, const void *rgbPixels)
{
	//CheckAllBlack(width, height, rgbPixels);
	stbi_flip_vertically_on_write(YES);
	stbi_write_png(fileName, width, height, 3, rgbPixels, width * 3);
	printf("Wrote %s\n", fileName);
}

ze_external void ZEPlatform_SaveImageRGBA(
	const char *fileName, i32 width, i32 height, const void *rgbaPixels)
{
	//CheckAllBlack(width, height, rgbPixels);
	stbi_flip_vertically_on_write(YES);
	stbi_write_png(fileName, width, height, 3, rgbaPixels, width * 4);
	printf("Wrote %s\n", fileName);
}


#endif // ZE_ASSETS_H