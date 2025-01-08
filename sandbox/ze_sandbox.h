#ifndef ZE_SANDBOX_H
#define ZE_SANDBOX_H

#include <zengine.h>

ze_internal ZRDataTexture g_data;

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

ze_internal i32 Frame(f64 delta)
{
	local_persist f32 degrees = 0;
	degrees += 90.f * (f32)delta;
	
	// write some crap to the data texture.
	g_data.Clear();
	f32 radians = degrees * DEG2RAD;
	f32 x = cosf(radians);
	f32 y = sinf(radians);
	g_data.WriteItem({}, { 0, 0, 1.0, 1.0 }, { 1, 1, 1, 1});
	f32 minX, minY, maxX, maxY;
	ZR_GetAsciiUVs('a', &minX, &minY, &maxX, &maxY);
	g_data.WriteItem({x, y, 0, radians}, { minX, minY, maxX, maxY }, { 1, 1, 1, 1});
	minX, minY, maxX, maxY;
	ZR_GetAsciiUVs('1', &minX, &minY, &maxX, &maxY);
	g_data.WriteItem({-x, -y, 0, radians}, { minX, minY, maxX, maxY }, { 1, 1, 1, 1});
	//data.WriteItem({1.5f, 1.5f, 0, -15.f}, { 0, 0, 1.0, 1.0 }, { 1, 1, 1, 1});
	//data.WriteItem({-2.f, -1.f, 0, 0}, { 0, 0, 1.0, 1.0 }, { 1, 1, 1, 1});
	//PrintDataTex(g_data);

	#if 0
	ZR_DrawTest();
	#else
	f32 prj[16];
	f32 screenHeight = 2.0f;
	M4x4_ToIdentity(prj);
	ZEWindowInfo window = Platform_GetWindowInfo();
    M4x4_SetupOrthoProjection(prj, screenHeight, window.aspectRatio);
    f32 view[16];
	M4x4_ToIdentity(view);
    view[M4x4_W2] = -1;
	ZR_BeginFrame(0.5f, 0.0f, 0.5f);
	ZR_DrawQuadBatch(prj, view, &g_data);
	#endif

	ZR_SubmitFrame();
	return YES;
}

ze_internal void FrameTick(ZEFrame frame)
{
	Frame(frame.delta);
}

ze_internal void KeyCallback(i32 key, i32 value, i32 mods)
{
	printf("App saw engine key %d, action %d, mods %d\n", key, value, mods);
}

ze_internal void Sandbox_Run()
{
	ZEApp app = {};
	app.windowName = "Zealous Engine Sandbox";
	app.frameCallback = FrameTick;
	app.keyCallback = KeyCallback;

	zErrorCode err =  ZE_EngineStart(app);
	if (err != ZERROR_CODE_NONE)
	{
		printf("Engine startup failed with code %d\n", err);
		Sleep(2000);
		return;
	}
    
    printf("Zealous Engine Sandbox\n");
	g_data = ZR_AllocDataTexture();
	ZERunLoop(60, FrameTick);
	printf("Sandbox test complete\n");
}

#endif // ZE_SANDBOX_H