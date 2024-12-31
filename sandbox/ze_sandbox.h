#ifndef ZE_SANDBOX_H
#define ZE_SANDBOX_H

#include <zengine.h>

internal f32 g_prim_quadVerts[] =
{
	-0.5, -0.5,  0,
	 0.5, -0.5,  0,
	 0.5,  0.5,  0,

	-0.5, -0.5,  0,
	 0.5,  0.5,  0,
	-0.5,  0.5,  0
};

internal f32 g_prim_quadUVs[] =
{
	0, 0,
	1, 0,
	1, 1,

	0, 0,
	1, 1,
	0, 1
};

internal f32 g_prim_quadNormals[] =
{
	0,  0,  -1,
	0,  0,  -1,
	0,  0,  -1,

	0,  0,  -1,
	0,  0,  -1,
	0,  0,  -1
};

static void Sandbox_Run()
{
    printf("Zealous Engine Sandbox\n");
    /*
    i32 imgSize = 32;
    i32 numPixels = imgSize * imgSize;
    u8* pixels = (u8*)Platform_Alloc(numPixels * 4);
    for (int i = 0; i < numPixels; i += 4)
    {
        pixels[i + 0] = 255;
        pixels[i + 1] = 0;
        pixels[i + 2] = 255;
        pixels[i + 3] = 255;
    }
    u32 texHandle;
    ZE_UploadTexture(pixels, imgSize, imgSize, &texHandle);
    u32 vaoHandle = 0;
    u32 vboHandle = 0;

    ZE_UploadMesh(6, g_prim_quadVerts, g_prim_quadUVs, g_prim_quadNormals, &vaoHandle, &vboHandle);
    //ZE_UploadTexture()
	*/
}

#endif // ZE_SANDBOX_H