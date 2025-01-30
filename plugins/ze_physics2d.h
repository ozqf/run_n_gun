#ifndef ZE_PHYSICS2D_H
#define ZE_PHYSICS2D_H
/*
2D physics public interface
Any 2D physics implementations should use this interface
*/
#define ZP_SHAPE_TYPE_BOX 0
#define ZP_SHAPE_TYPE_CIRCLE 1

#define ZP_ERROR_NONE 0
#define ZP_ERROR_NOT_FOUND 1

#define ZP_MASK_ALL 65535

#define ZP_EMPTY_ID 0

#include <zengine.h>
struct ZPVec2
{
	f32 x, y;
};


struct ZPTransform2d
{
    ZPVec2 pos;
    f32 radians;
};

struct ZPBodyState
{
    ZPTransform2d t;
    ZPVec2 velocity;
    f32 angularVelocity;
    i32 externalId;
};

struct ZPShapeDef
{
    ZPVec2 pos;
    ZPVec2 radius;
    // if shapetype == circle, only x component of size is used.
    i32 shapeType;
};

struct ZPBodyDef
{
    i32 externalId;
    i32 bIsStatic;
    i32 bLockRotation;
    f32 resitition;
    f32 friction;
    u16 categoryBits;
    u16 maskBits;
    ZPShapeDef shape;
};

struct ZAABBResult
{
    i32 volumeId;
    i32 externalId;
};

struct ZPRaycastResult
{
    ZPVec2 pos;
    ZPVec2 normal;
    f32 fraction;
    i32 volumeId;
    i32 externalId;
    u16 categoryBits;
};

struct ZPStats
{
    int numRegisteredDynamic;
    int numRegisteredStatic;
    int numBodies;
};

typedef void* (*ZP_mallocFunction)(const zeSize numBytes);
typedef void* (*ZP_reallocFunction)(void* memory,  const zeSize numBytes);
typedef void (*ZP_freeFunction)(void * memory);

struct ZPPlatform
{
    ZP_mallocFunction malloc;
    ZP_reallocFunction realloc;
    ZP_freeFunction free;
};

// construct
// ze_external zeHandle ZP_AddDynamicVolume(ZPShapeDef def);
ze_external zeHandle ZP_AddStaticVolume(
    ZPVec2 pos, ZPVec2 size, u16 categoryBits, u16 maskBits);
ze_external zeHandle ZP_AddBody(ZPBodyDef def);
ze_external zErrorCode ZP_RemoveBody(zeHandle bodyId);

// affect
ze_external void ZP_ApplyForce(zeHandle bodyId, ZPVec2 force);
ze_external void ZP_ApplyForceAtPoint(zeHandle bodyId, ZPVec2 force, ZPVec2 point);
ze_external void ZP_SetBodyPosition(zeHandle bodyId, ZPVec2 pos);
ze_external void ZP_SetLinearVelocity(zeHandle bodyId, ZPVec2 v);
ze_external void ZP_SetBodyState(zeHandle bodyId, ZPBodyState state);
ze_external void ZP_SetBodyMaskBit(zeHandle bodyId, u16 maskBit, i32 bOn);

// query
ze_external ZPShapeDef ZP_GetBodyShape(zeHandle bodyId);
ze_external ZPTransform2d ZP_GetBodyPosition(zeHandle bodyId);
ze_external ZPBodyState ZP_GetBodyState(zeHandle bodyId);
// returns number of results
ze_external i32 ZP_Raycast(
    ZPVec2 from, ZPVec2 to, ZPRaycastResult* results, i32 maxResults, u16 mask);
// returns number of results.
ze_external i32 ZP_AABBCast(
	ZPVec2 min, ZPVec2 max, ZAABBResult* results, i32 maxResults, u16 mask);
ze_external i32 ZP_GroundTest(zeHandle physicsBody, u16 mask);

// lifetime
ze_external void ZPhysicsInit(ZPPlatform platform);
ze_external void ZPhysicsTick(f32 delta);
ze_external ZPStats ZPhysicsStats();

#endif ZE_PHYSICS2D_H
