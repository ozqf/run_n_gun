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

ze_internal void PrintDataTex(ZRDataTexture data)
{
	i32 itemIndex = 0;
	for (i32 i = 0; i < data.index; i += data.stride)
	{
		Vec4 pos = data.pixels[i];
		Vec4 uvs = data.pixels[i + 1];
		Vec4 scale = data.pixels[i + 2];
		printf("%d: pos %.3f, %.3f, %.3f. UVs %.3f, %.3f, %.3f, %.3f. Scale %.3f, %.3f\n",
			itemIndex, pos.x, pos.y, pos.z,
			uvs.x, uvs.y, uvs.z, uvs.w,
			scale.x, scale.y
		);
		itemIndex++;
	}
}

ze_internal void Sandbox_Run()
{
    printf("Zealous Engine Sandbox\n");
	ZRDataTexture data = ZRGL_AllocDataTexture();
	// write some crap to the data texture.
	f32 minX, minY, maxX, maxY;
	ZR_GetAsciiUVs('a', &minX, &minY, &maxX, &maxY);
	data.WriteItem({-2.f, -1.f, 0, 45.f}, { minX, minY, maxX, maxY }, { 1, 1, 1, 1});
	data.WriteItem({1.5f, 1.5f, 0, -15.f}, { 0, 0, 1.0, 1.0 }, { 1, 1, 1, 1});
	//data.WriteItem({-2.f, -1.f, 0, 0}, { 0, 0, 1.0, 1.0 }, { 1, 1, 1, 1});
	PrintDataTex(data);

	#if 0
	ZR_DrawTest();
	#else

	f32 prj[16];
	f32 screenHeight = 1.0f;
	M4x4_ToIdentity(prj);
    M4x4_SetupOrthoProjection(prj, screenHeight, Window_GetAspectRatio());

    f32 view[16];
	M4x4_ToIdentity(view);
    view[M4x4_W2] = -1;
	ZR_BeginFrame(0.5f, 0.0f, 0.5f);
	ZR_DrawQuadBatch(prj, view, &data);
	#endif
	ZR_SubmitFrame();
	
	printf("Sandbox test complete\n");
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