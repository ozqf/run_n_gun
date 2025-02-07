
#include <ze_sandbox.h>

#define MODE_SMALL_BATCH 0
#define MODE_LARGE_BATCH 1
#define MODE_PHYSICS_TEST 2

ze_internal i32 _mode = 0;

ze_internal ZRDataTexture g_data;

struct Sprite
{
	f32 x, y, z, radians, scaleX, scaleY;
	f32 velX, velY;
	char c;
};

ze_internal Sprite* _sprites;
ze_internal i32 _maxSprites;
ze_internal i32 _nextSprite;
ze_internal f32 _spriteView[16];

ze_internal f32 Randf()
{
	f32 r = (f32)rand();
	const f32 max = (f32)RAND_MAX;
	return r / max;
}

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

ze_internal i32 SmallBatch_Frame(f64 delta)
{
	local_persist f32 degrees = 0;
	degrees += 90.f * (f32)delta;
	
	// write some crap to the data texture.
	g_data.Clear();
	f32 radians = degrees * DEG2RAD;
	f32 x = cosf(radians);
	f32 y = sinf(radians);
	
	f32 minX, minY, maxX, maxY;
	f32 charWidth = 1.f / 16.f;
	minX = charWidth;
	maxX = charWidth * 2;

	minY = 1.0f - (charWidth * 5.f);
	maxY = 1.0f - (charWidth * 4.f);
	
	g_data.WriteItem({}, { 1, 1, 1, 1}, { minX, minY, maxX, maxY }, { 1, 1, 1, 1});
	ZR_GetAsciiUVs('A', &minX, &minY, &maxX, &maxY);
	g_data.WriteItem({x, y, 0, radians}, { 0.5f, 0.5f, 0.5f, 1}, { minX, minY, maxX, maxY }, { 1.0f, 0.1f, 0.1f, 1});
	g_data.WriteItem({-x, -y, 0, radians}, { 1, 1, 1, 1}, { 0, 0, 1, 1 }, { 1, 1, 1, 1});
	
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
	ZR_DrawQuadBatch(prj, view, &g_data);
	#endif

	return YES;
}

ze_internal void LargeBatch_Frame(f32 delta)
{
	g_data.Clear();

	for (i32 i = 0; i < _nextSprite; ++i)
	{
		Sprite* spr = &_sprites[i];
		f32 minX, minY, maxX, maxY;
		ZR_GetAsciiUVs(spr->c, &minX, &minY, &maxX, &maxY);
		spr->radians = spr->radians + ((spr->velX * DEG2RAD) * delta);
		g_data.WriteItem(
            {spr->x, spr->y, 0, spr->radians},
            { 1, 1, 1, 1}, 
            {minX, minY, maxX, maxY},
            {1, 1, 1, 1}
        );
	}

	f32 prj[16];
	f32 screenHeight = 8.0f;
	M4x4_ToIdentity(prj);
	ZEWindowInfo window = Platform_GetWindowInfo();
    M4x4_SetupOrthoProjection(prj, screenHeight, window.aspectRatio);
    f32 view[16];
	M4x4_ToIdentity(view);
    view[M4x4_W2] = -1;
	
	ZR_DrawQuadBatch(prj, view, &g_data);
	return;
}

ze_internal void LargeBatch_InitMode()
{
	srand(3);
	_maxSprites = 2048;
	_sprites = (Sprite*)Platform_Alloc(sizeof(Sprite) * _maxSprites);
	_nextSprite = 0;
	
	for (i32 i = 0; i < 1024; ++i)
	{
		Sprite* spr = &_sprites[i];
		_nextSprite++;

		f32 rx = Randf();
		f32 ry = Randf();
		f32 rc = Randf();
		f32 rRot = Randf();
		spr->x = -8 + (rx * 16.f);
		spr->y = -8 + (ry * 16.f);
		//printf("%.3f, %.3f from rand %.3f, %.3f\n", spr->x, spr->y, rx, ry);
		spr->c = (char)(rc * 128);
		spr->velX = -360.f + (rRot * (360 * 2.f));
	}
}

ze_internal void FrameTick(ZEFrame frame)
{

	ZR_BeginFrame(0.1f, 0.1f, 0.1f);
	switch (_mode)
	{
		case MODE_LARGE_BATCH:
			LargeBatch_Frame(frame.delta);
			break;
        case MODE_PHYSICS_TEST:
            PhysicsTestFrame(frame);
            break;
		default:
			SmallBatch_Frame(frame.delta);
			break;
	}
	ZR_SubmitFrame();
}

ze_internal void SetMode(i32 newMode)
{
	switch (newMode)
	{
		case MODE_SMALL_BATCH:
		_mode = MODE_SMALL_BATCH;
		break;
		case MODE_LARGE_BATCH:
		_mode = MODE_LARGE_BATCH;
		break;
		case MODE_PHYSICS_TEST:
		_mode = MODE_PHYSICS_TEST;
		PhysicsTest_On();
		break;
		default:
		_mode = MODE_SMALL_BATCH;
		break;
	}
}

ze_internal void KeyCallback(i32 key, i32 value, i32 mods)
{
	printf("App saw engine key %d, action %d, mods %d\n", key, value, mods);
	if (key == Z_INPUT_CODE_1 && value != 0)
	{
		SetMode(MODE_SMALL_BATCH);
	}
	else if (key == Z_INPUT_CODE_2 && value != 0)
	{
		SetMode(MODE_LARGE_BATCH);
	}
    else if (key == Z_INPUT_CODE_3 && value != 0)
    {
		SetMode(MODE_PHYSICS_TEST);
    }
}

ze_external void Sandbox_Run(i32 argc, char** argv)
{
	ZEApp app = {};
	app.windowName = "Zealous Engine Sandbox";
	app.frameCallback = FrameTick;
	app.keyCallback = KeyCallback;

	zErrorCode err =  ZE_EngineStart(app, argc, argv);
	if (err != ZERROR_CODE_NONE)
	{
		printf("Engine startup failed with code %d\n", err);
		Platform_Sleep(2000);
		return;
	}

    printf("Zealous Engine Sandbox\n");
	g_data = ZR_AllocDataTexture();
	LargeBatch_InitMode();
	SetMode(MODE_PHYSICS_TEST);
	ZERunLoop(60, FrameTick);
	printf("Sandbox test complete\n");
}
