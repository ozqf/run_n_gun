
#include <zengine.h>
#include <ze_physics2d.h>

const i32 screenSize = 8;

ze_internal u32 _frameCount = 0;

ze_internal ZRDataTexture g_menuTex;

ze_internal zErrorCode Init()
{
    ZPPlatform physPlat = {};
	physPlat.malloc = Platform_Alloc;
	physPlat.free = Platform_Free;
	physPlat.realloc = Platform_Realloc;
	ZPhysicsInit(physPlat);
    g_menuTex = ZR_AllocDataTexture();
    return ZERROR_CODE_NONE;
}

ze_internal void DrawMenuChars()
{    g_menuTex.Clear();

    char* menuText = "RUN N GUN\nMAIN MENU\n> START <\nEXIT\n";
    Point2 size = ZE_MeasureTextBlock(menuText);
    //printf("%d, %d\n", size.x, size.y);
    i32 i = 0;
    char c = menuText[i];
    f32 sizeX = 1;
    f32 sizeY = 1;
    f32 originX = -4;
    f32 originY = 4;
    f32 x = originX;
    f32 y = originY;
    while (c != '\0')
    {
        if (c == '\n')
        {
            y -= sizeY;
            x = originX;
        }
        else
        {
            f32 minX, minY, maxX, maxY;
		    ZR_GetAsciiUVs(c, &minX, &minY, &maxX, &maxY);
            g_menuTex.WriteItem(
                {x, y, 0, 0},
                { sizeX, sizeY, 1, 1},
                {minX, minY, maxX, maxY},
                {1, 1, 1, 1});
            x += sizeX;
        }
        i++;
        c = menuText[i];
    }
    
	f32 prj[16];
	f32 screenHeight = 8.0f;
	M4x4_ToIdentity(prj);
	ZEWindowInfo window = Platform_GetWindowInfo();
    M4x4_SetupOrthoProjection(prj, screenHeight, window.aspectRatio);
    f32 view[16];
	M4x4_ToIdentity(view);
    view[M4x4_W2] = -1;

    ZR_DrawQuadBatch(prj, view, &g_menuTex);
}

ze_internal void DrawFrame()
{
    ZR_BeginFrame(0.2f, 0.2f, 0.2f);
    DrawMenuChars();
	ZR_SubmitFrame();
}

/**************************************
 * engine callbacks
 */
ze_internal void FrameTick(ZEFrame frame)
{
    _frameCount += 1;
	// if (ZEGetSpecialMode() == SPECIAL_APP_MODE_SINGLE_SHOT && _frameCount > 2)
	// {
	// 	Platform_Screenshot("screenshot.png");
	// 	Platform_Shutdown();
	// 	return;
	// }
    if (_frameCount % 60 == 0)
    {
        printf("Tock %d\n", _frameCount);
    }
    DrawFrame();
}

ze_internal void KeyCallback(i32 key, i32 value, i32 mods)
{
    printf("App saw engine key %d, action %d, mods %d\n", key, value, mods);
    if (key == 39)
    {
        Platform_Shutdown();
    }
}

ze_external void rng_start(int argc, char** argv)
{
    printf("Run N Gun\n");
    ZEApp app = {};
	app.windowName = "Run N Gun";
	app.frameCallback = FrameTick;
	app.keyCallback = KeyCallback;
    app.flags = APP_FLAG_OVERRIDE_ESCAPE_KEY;

	zErrorCode err = ZE_EngineStart(app, argc, argv);

    err = Init();
    if (err != ZP_ERROR_NONE)
    {
        Platform_ErrorBox("Error in RnG Init");
        return;
    }
    
    printf("Entering frame loop...\n");
    ZERunLoop(60, FrameTick);
    printf("...exiting frame loop\n");
}

