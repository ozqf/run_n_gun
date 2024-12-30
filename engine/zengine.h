#ifndef ZENGINE_H
#define ZENGINE_H

#include <stdint.h>	// for types
#include <stdio.h>	// for printf
#include <string.h>	// for memcpy

///////////////////////////////////////////////////////////////////////
// PRIMITIVE TYPES
///////////////////////////////////////////////////////////////////////
#define f32 float
#define f64 double

#define zeSize size_t

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define u8 uint8_t
#define uChar uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define bool32 uint32_t

#define YES 1
#define NO 0

typedef i64 frameInt;
typedef f64 timeFloat;
typedef i32 zErrorCode;
typedef i32 zeHandle;       // generic resource Id type
typedef u64 zMemOffset;		// an offset to a position in a buffer.
#define ZE_EMPTY_HANDLE 0

static_assert(sizeof(char) == 1, "Code requires char size == 1");
static_assert(sizeof(i8) == 1, "Code requires i8 size == 1");
static_assert(sizeof(u8) == 1, "Code requires u8 size == 1");
static_assert(sizeof(i16) == 2, "Code requires i16 size == 2");
static_assert(sizeof(u16) == 2, "Code requires u16 size == 2");
static_assert(sizeof(i32) == 4, "Code requires int size == 4");
static_assert(sizeof(u32) == 4, "Code requires uint size == 4");

static_assert(sizeof(f32) == 4, "Code requires f32 size == 4");
static_assert(sizeof(f64) == 8, "Code requires f64 size == 8");

#define internal static
#define local_persist static
#define ze_internal static
#define ze_external extern "C"

///////////////////////////////////////////////////////////////////////
// ERROR CODES
///////////////////////////////////////////////////////////////////////
#define ZERROR_CODE_NONE 0
#define ZERROR_CODE_UNSPECIFIED 1
#define ZERROR_CODE_BAD_PARAMETER 2
#define ZERROR_CODE_NO_CAPACITY 3

///////////////////////////////////////////////////////////////////////
// PRIMITIVE FUNCTIONS
///////////////////////////////////////////////////////////////////////
inline zeSize ZE_Copy(void *dest, void *source, zeSize numBytes)
{
	memcpy(dest, source, numBytes);
	return numBytes;
}

///////////////////////////////////////////////////////////
// Embedded assets
///////////////////////////////////////////////////////////
// unit cube, -0.5 to 0.5 on x/y/z
#define ZE_EMBEDDED_CUBE_NAME "cube"
// unit quad, -0.5 to 0.5 on x/y
#define ZE_EMBEDDED_QUAD_NAME "quad"
// quad -1 to 1 on x/y to cover screenspace.
#define ZE_EMBEDDED_SCREEN_QUAD_NAME "screen_quad"

#define FALLBACK_MATERIAL_NAME "debug_magenta"
#define FALLBACK_CHEQUER_MATERIAL "debug_chequer"

#define FALLBACK_TEXTURE_NAME "fallback_texture"
#define FALLBACK_TEXTURE_WHITE "fallback_texture_white"
#define FALLBACK_CHARSET_TEXTURE_NAME "fallback_charset"
#define FALLBACK_CHARSET_SEMI_TRANSPARENT_TEXTURE_NAME "fallback_charset_semi_transparent"

#define DEFAULT_SHADER_FLAG_ALBEDO_COLOUR_ONLY (1 << 0)

// 32 bit colours
#define COLOUR_U32_EMPTY { 0, 0, 0, 0 }
#define COLOUR_U32_WHITE { 255, 255, 255, 255 }
#define COLOUR_U32_BLACK { 0, 0, 0, 255 }
#define COLOUR_U32_RED { 255, 0, 0, 255 }
#define COLOUR_U32_GREEN { 0, 255, 0, 255 }
#define COLOUR_U32_BLUE { 0, 0, 255, 255 }
#define COLOUR_U32_YELLOW { 255, 255, 0, 255 }
#define COLOUR_U32_CYAN { 0, 255, 255, 255 }
#define COLOUR_U32_PURPLE { 255, 0, 255, 255 }
#define COLOUR_U32_GREY_LIGHT { 200, 200, 200, 255 }
#define COLOUR_U32_GREY { 100, 100, 100, 255 }
#define COLOUR_U32_GREY_DARK { 50, 50, 50, 255 }

#define COLOUR_U32_SEMI_GREY { 155, 155, 155, 155 }
#define COLOUR_U32_SEMI_GREY_DARK { 200, 200, 200, 200 }

// colors as Vec4s
#define COLOUR_F32_EMPTY { 0.f, 0.f, 0.f, 0.f }
#define COLOUR_F32_WHITE { 1.f, 1.f, 1.f, 1.f }
#define COLOUR_F32_BLACK { 0.f, 0.f, 0.f, 1.f }
#define COLOUR_F32_RED { 1.f, 0.f, 0.f, 1.f }
#define COLOUR_F32_GREEN { 0.f, 1.f, 0.f, 1.f }
#define COLOUR_F32_BLUE { 0.f, 0.f, 1.f, 1.f }
#define COLOUR_F32_YELLOW { 1.f, 1.f, 0.f, 1.f }
#define COLOUR_F32_CYAN { 0.f, 1.f, 1.f, 1.f }
#define COLOUR_F32_PURPLE { 1.f, 0.f, 1.f, 1.f }

#define COLOUR_F32_ORANGE { 1.f, 0.5f, 0.f, 1.f }
#define COLOUR_F32_LIGHT_GREY { 0.5f, 0.5f, 0.5f, 1.f }

#define ZR_TEX_SAMPLER_DEFAULT 0
#define ZR_DEFAULT_FOV 100.f

#define KILOBYTES(bytes) ((bytes)*1024LL)
#define MEGABYTES(bytes) (KILOBYTES(bytes) * 1024LL)
#define BytesAsKB(bytes) (bytes / 1024LL)
#define BytesAsMB(bytes) (bytes / (1024LL * 1024LL))

#define F32_EPSILON 1.19209290E-07F // decimal constant
#define ZALMOST_ZERO(value) (value < F32_EPSILON && value > -F32_EPSILON)

#define ZFLOORF(f) floorf(f)

// shorten some bitwise stuff
#define IF_BIT(uintFlags, uintBit) ((uintFlags & uintBit) != 0)
#define IF_TO_BIT(bitCondition, uintFlags, uintBit) \
	{                                               \
		if (bitCondition)                           \
		{                                           \
			uintFlags |= uintBit;                   \
		}                                           \
		else                                        \
		{                                           \
			uintFlags &= ~uintBit;                  \
		}                                           \
	}

#define pi32 3.14159265359f
#define DEG2RAD 3.141593f / 180.0f
#define RAD2DEG 57.2958f
#define FULL_ROTATION_RADIANS (pi32 * 2)
#define Z_INFINITY 0x7F800000

#define ZABS(value) (value = (value >= 0 ? value : -value))
#define ZMIN(x, y) ((x) < (y) ? (x) : (y))
#define ZMAX(x, y) ((x) > (y) ? (x) : (y))

///////////////////////////////////////////////////////////////////////
// DATA TYPES
///////////////////////////////////////////////////////////////////////

union ColourU32
{
	u8 array[4];
	struct
	{
		u8 red, green, blue, alpha;
	};
	struct
	{
		u8 r, g, b, a;
	};
	struct
	{
		u32 value;
	};
};


union ColourF32
{
	f32 array[4];
	struct
	{
		f32 red, green, blue, alpha;
	};
	struct
	{
		f32 r, g, b, a;
	};
};

#define VEC3_SIZE = 12
#define VEC2_SIZE = 8

struct Vec2
{
	union
	{
		struct
		{
			f32 x, y;
		};
		f32 parts[2];
	};
};

struct Vec3
{
	union
	{
		struct
		{
			f32 x, y, z;
		};
		f32 parts[3];
	};
	// f32 x, y, z, w;
	// // overload array operator to return a pointer to x + index
	// f32 &operator[](int index) { return ((&x)[index]); }
};

const Vec3 vec3_zero = {};

struct Vec4
{
	union
	{
		struct
		{
			f32 x, y, z, w;
		};
		f32 e[4];
	};
	// f32 x, y, z, w;
	// // overload array operator to return a pointer to x + index
	// f32 &operator[](int index) { return ((&x)[index]); }
};

///////////////////////////////////////////////////////////
// Asset data types
///////////////////////////////////////////////////////////

#define ZE_ASSET_TYPE_NONE 0
#define ZE_ASSET_TYPE_TEXTURE 1
#define ZE_ASSET_TYPE_MESH 2
#define ZE_ASSET_TYPE_MATERIAL 3
#define ZE_ASSET_TYPE_BLOB 4

struct ZRAsset
{
	i32 id;
	i32 index;
	i32 type;
	// Data has changed - needs to be re-uploaded
	i32 bIsDirty;
	zeSize totalSize;
	zMemOffset nameOffset;
	char *fileName;
	i32 sentinel;
};

struct ZRTextureHeader
{
	i32 width;
	i32 height;
	ColourU32 *data;
};

struct ZRBlobAsset
{
	ZRAsset header;
	void* ptrA;
	zeSize sizeA;
	void* ptrB;
	zeSize sizeB;
};

struct ZRMaterial
{
	ZRAsset header;
	i32 programId;
	i32 diffuseTexId;
	i32 emissionTexId;
};

struct ZRTexture
{
	ZRAsset header;
	i32 width;
	i32 height;
	ColourU32 *data;
};

struct ZRQuad
{
	i32 textureId;
	Vec2 offset;
	Vec2 uvMin;
	Vec2 uvMax;
	f32 radians;
	ColourF32 colour;
};

struct ZRLineVertex
{
	Vec3 pos;
	Vec3 colour;
	f32 thickness;
};

// Currently stores every vertex, no sharing
struct ZRMeshData
{
	u32 numVerts;
	// dynamic mesh may have more capacity.
	u32 maxVerts;

	f32 *verts;
	f32 *uvs;
	f32 *normals;

	Vec3 *GetVert(i32 i)
	{
		u8 *mem = (u8 *)verts;
		mem += (sizeof(Vec3) * i);
		return (Vec3 *)mem;
	}

	void Clear()
	{
		this->numVerts = 0;
	}

	void AddTri(
		Vec3 v0, Vec3 v1, Vec3 v2,
		Vec2 uv0, Vec2 uv1, Vec2 uv2,
		Vec3 n0, Vec3 n1, Vec3 n2)
	{
		i32 i = this->numVerts;
		this->numVerts += 3;
		// step to
		i32 vertStride = sizeof(f32) * 3 * i;
		i32 uvStride = sizeof(f32) * 2 * i;
		Vec3 *vert = (Vec3 *)((u8 *)verts + vertStride);
		Vec2 *uv = (Vec2 *)((u8 *)uvs + uvStride);
		Vec3 *normal = (Vec3 *)((u8 *)normals + vertStride);
		vert[0] = v0;
		vert[1] = v1;
		vert[2] = v2;
		uv[0] = uv0;
		uv[1] = uv1;
		uv[2] = uv2;
		normal[0] = n0;
		normal[1] = n1;
		normal[2] = n2;
	}

	void AddVert(
		Vec3 vert,
		Vec2 uv,
		Vec3 normal)
	{
		if (this->numVerts >= this->maxVerts)
		{
			return;
		}
		i32 i = this->numVerts;
		this->numVerts += 1;
		// step to
		i32 vertStride = sizeof(Vec3) * i;
		i32 uvStride = sizeof(Vec2) * i;
		Vec3 *vertPtr = (Vec3 *)((u8 *)verts + vertStride);
		Vec2 *uvPtr = (Vec2 *)((u8 *)uvs + uvStride);
		Vec3 *normalPtr = (Vec3 *)((u8 *)normals + vertStride);
		vertPtr[0] = vert;
		uvPtr[0] = uv;
		normalPtr[0] = normal;
	}

	i32 MeasureBytes()
	{
		i32 bytes = 0;
		const i32 v3size = sizeof(f32) * 3;
		const i32 v2size = sizeof(f32) * 2;
		bytes += v3size * numVerts;
		bytes += v2size * numVerts;
		bytes += v3size * numVerts;
		return bytes;
	}

	i32 CopyData(ZRMeshData original)
	{
		if (original.numVerts > maxVerts)
		{
			printf("No space to copy mesh! %d verts have %d\n",
				   original.numVerts, maxVerts);
			return ZERROR_CODE_NO_CAPACITY;
		}
		// no need to set max verts, we assume we have the capacity for it
		numVerts = original.numVerts;

		i32 numVertBytes = (sizeof(f32) * 3) * numVerts;
		i32 numUVSBytes = (sizeof(f32) * 2) * numVerts;
		printf("Copying %d verts (%d vert bytes, %d uv bytes, %d normal bytes)\n",
			   numVerts, numVertBytes, numUVSBytes, numVertBytes);
		// i32 numFloats = numVerts * 3;
		// for (i32 i = 0; i < numFloats; ++i)
		// {
		//     this->verts[i] = original.verts[i];
		// }
		ZE_Copy(this->verts, original.verts, numVertBytes);
		ZE_Copy(this->uvs, original.uvs, numUVSBytes);
		ZE_Copy(this->normals, original.normals, numVertBytes);

		// ZE_CompareMemory((u8 *)verts, (u8 *)(original.verts), );
		return ZERROR_CODE_NONE;
	}

	void PrintVerts()
	{
		printf("--- %d of %d verts ---\n", numVerts, maxVerts);
		f32 *cursor = verts;
		for (u32 i = 0; i < numVerts; ++i)
		{
			printf("%d: %.3f, %.3f, %.3f\n", i, cursor[0], cursor[1], cursor[2]);
			cursor += 3;
		}
	}
};

struct ZRMeshAsset
{
	ZRAsset header;
	ZRMeshData data;
};

///////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////
ze_external zErrorCode ZE_EngineInit();

ze_external ZRTexture *ZAssets_AllocTex(i32 width, i32 height, char* name);

ze_external void* Platform_Alloc(zeSize size);
ze_external void* Platform_Realloc(void* ptr, zeSize size);
ze_external void Platform_Free(void* ptr);

#endif // ZENGINE_H