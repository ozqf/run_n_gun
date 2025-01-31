
#include <ze_sandbox.h>
#include <ze_physics2d.h>

ze_internal i32 _initialised = NO;

#define MAX_WORLD_VOLUMES 32
ze_internal i32 _nextWorldVolume = 0;
ze_internal i32 _worldVolumes[MAX_WORLD_VOLUMES];

ze_internal u32 _white32x32Pixels[32 * 32];
ze_internal u32 _white32x32Handle = 0;

ze_internal ZRDataTexture _dataTex;

ze_external void PhysicsTest_On()
{
    if (_initialised) { return; }
    _initialised = true;
    printf("Hello Physics Init \n");
	_dataTex = ZR_AllocDataTexture();

    for (int i = 0; i < (32 * 32); ++i)
    {
        _white32x32Pixels[i] = 0xFFFFFFFF;
    }
    ZR_UploadTexture((u8*)_white32x32Pixels, 32, 32, &_white32x32Handle, NO);
    
	ZPPlatform physPlat = {};
	physPlat.malloc = Platform_Alloc;
	physPlat.free = Platform_Free;
	physPlat.realloc = Platform_Realloc;
	ZPhysicsInit(physPlat);
    
    _worldVolumes[_nextWorldVolume++] = ZP_AddStaticVolume({0, -8}, {20, 1}, 1, 1);
    _worldVolumes[_nextWorldVolume++] = ZP_AddStaticVolume({-10, 0}, {1, 16}, 1, 1);
    _worldVolumes[_nextWorldVolume++] = ZP_AddStaticVolume({10, 0}, {1, 16}, 1, 1);

    ZPBodyDef body = {};
    body.bIsStatic = NO;
    body.bLockRotation = NO;
    body.categoryBits = 1;
    body.maskBits = 1;
    body.externalId = _nextWorldVolume++;
    body.shape.shapeType = ZP_SHAPE_TYPE_BOX;
    body.shape.radius = { 1, 1 };
    body.shape.pos = { 0, 4 };
    _worldVolumes[body.externalId] = ZP_AddBody(body);
    //_worldVolumes[_nextWorldVolume++] = ZP_AddStaticVolume({5, 0}, {1, 8}, 1, 1);

    printf("Hello Physics created %d world volumes \n", _nextWorldVolume);
}

ze_external void PhysicsTestFrame(ZEFrame frame)
{
    ZPhysicsTick(frame.delta);
    #if 1
    _dataTex.Clear();

	for (i32 i = 0; i < _nextWorldVolume; ++i)
    {
        ZPBodyState body = ZP_GetBodyState(_worldVolumes[i]);
        //body.t.pos
        Vec4 pos = { body.t.pos.x, body.t.pos.y, 0, body.t.radians };
        Vec4 uvs = { 0, 0, 1, 1 };
        Vec4 colour;
        if (i < 3)
        {
            colour = { 0.5, 0.5, 0.5, 1 };
        }
        else
        {
            colour = { 1.0, 1.0, 0.0, 1 };
        }
        Vec4 scale = { body.size.x, body.size.y, 1, 1 };
        _dataTex.WriteItem(pos, scale, uvs, colour);
    }
    
	f32 prj[16];
	f32 screenHeight = 8.0f;
	M4x4_ToIdentity(prj);
	ZEWindowInfo window = Platform_GetWindowInfo();
    M4x4_SetupOrthoProjection(prj, screenHeight, window.aspectRatio);
    f32 view[16];
	M4x4_ToIdentity(view);
    view[M4x4_W2] = -1;
	_dataTex.diffuseHandle = _white32x32Handle;
    ZR_DrawQuadBatch(prj, view, &_dataTex);

    #endif
}
