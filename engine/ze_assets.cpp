#include <zengine.h>

#define PILE_SENTINEL 0xDEADBEEF

internal u8* _pile;
internal zeSize _pileSize;
internal zMemOffset _pileCursor = 0;
// first is 1, 0 considered invalid
internal i32 _nextId = 1;

internal void AllocOnPile(zeSize objectSize, u8** outPtr, i32* outId)
{
    *outId = _nextId++;
    // jump dist, id, object, sentinel 
    zeSize totalRequired = sizeof(zMemOffset) + objectSize + sizeof(i32) + sizeof(i32);
    zMemOffset start = _pileCursor;
    //zMemOffset* cursor = (zMemOffset*)(_pile + _pileCursor);
    //*cursor = totalRequired;
    *(zMemOffset*)(_pile + _pileCursor) = totalRequired;
    _pileCursor += sizeof(zMemOffset);

    *(i32*)(_pile + _pileCursor) = *outId;
    _pileCursor += sizeof(i32);
    
    *outPtr = (u8*)(_pile + _pileCursor);
    
    _pileCursor += objectSize;
    *(u32*)(_pile + _pileCursor) = PILE_SENTINEL;
    _pileCursor += sizeof(u32);
    printf("Allocated %lld bytes on pile. Id %d, obj size %lld. Cursor at %lld\n",
    totalRequired, *outId, objectSize, _pileCursor);
}

ze_external ZRTexture* ZAssets_AllocTex(i32 width, i32 height, char* name)
{
    zeSize pixelDataSize = width * height * sizeof(u32);
    zeSize headerSize = sizeof(u32) + sizeof(u32);
    u8* ptr;
    i32 id;
    AllocOnPile(pixelDataSize + headerSize, &ptr, &id);
    return NULL;
}

ze_external zErrorCode ZAssets_Init()
{
    _pileSize = MEGABYTES(32);
    _pile = (u8*)Platform_Alloc(_pileSize);

    return ZERROR_CODE_NONE;
}
