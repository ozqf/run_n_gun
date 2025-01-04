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


#define VEC_X 0
#define VEC_Y 1
#define VEC_Z 2
#define VEC_W 3

/* Matrix use
OpenGL uses column major, y/x matrices
/   0   4   8   12  \
|   1   5   9   13  |
|   2   6   10  14  |
\   3   7   11  15  /

*/

// Access Matrix Arrays
#define M3x3_X0 0
#define M3x3_X1 1
#define M3x3_X2 2

#define M3x3_Y0 3
#define M3x3_Y1 4
#define M3x3_Y2 5

#define M3x3_Z0 6
#define M3x3_Z1 7
#define M3x3_Z2 8

#define M4x4_X0 0
#define M4x4_X1 1
#define M4x4_X2 2
#define M4x4_X3 3

#define M4x4_Y0 4
#define M4x4_Y1 5
#define M4x4_Y2 6
#define M4x4_Y3 7

#define M4x4_Z0 8
#define M4x4_Z1 9
#define M4x4_Z2 10
#define M4x4_Z3 11

#define M4x4_W0 12
#define M4x4_W1 13
#define M4x4_W2 14
#define M4x4_W3 15

inline void M4x4_ToIdentity(f32* m)
{
	m[M4x4_X0] = 1;
    m[M4x4_X1] = 0;
    m[M4x4_X2] = 0;
    m[M4x4_X3] = 0;

    m[M4x4_Y0] = 0;
    m[M4x4_Y1] = 1;
    m[M4x4_Y2] = 0;
    m[M4x4_Y3] = 0;

    m[M4x4_Z0] = 0;
    m[M4x4_Z1] = 0;
    m[M4x4_Z2] = 1;
    m[M4x4_Z3] = 0;

    m[M4x4_W0] = 0;
    m[M4x4_W1] = 0;
    m[M4x4_W2] = 0;
    m[M4x4_W3] = 1;
}

inline void M4x4_Multiply(f32 *m0, f32 *m1, f32 *result)
{
	/*
                    0   4   8   12
                    1   5   9   13
                    2   6   10  14
                    3   7   11  15
                    |   |   |   |
    0   4   8   12-
    1   5   9   13-
    2   6   10  14-
    3   7   11  15-
    */

	f32 r[16];

	r[0] = (m0[0] * m1[0]) + (m0[4] * m1[1]) + (m0[8] * m1[2]) + (m0[12] * m1[3]);
	r[1] = (m0[1] * m1[0]) + (m0[5] * m1[1]) + (m0[9] * m1[2]) + (m0[13] * m1[3]);
	r[2] = (m0[2] * m1[0]) + (m0[6] * m1[1]) + (m0[10] * m1[2]) + (m0[14] * m1[3]);
	r[3] = (m0[3] * m1[0]) + (m0[7] * m1[1]) + (m0[11] * m1[2]) + (m0[15] * m1[3]);

	r[4] = (m0[0] * m1[4]) + (m0[4] * m1[5]) + (m0[8] * m1[6]) + (m0[12] * m1[7]);
	r[5] = (m0[1] * m1[4]) + (m0[5] * m1[5]) + (m0[9] * m1[6]) + (m0[13] * m1[7]);
	r[6] = (m0[2] * m1[4]) + (m0[6] * m1[5]) + (m0[10] * m1[6]) + (m0[14] * m1[7]);
	r[7] = (m0[3] * m1[4]) + (m0[7] * m1[5]) + (m0[11] * m1[6]) + (m0[15] * m1[7]);

	r[8] = (m0[0] * m1[8]) + (m0[4] * m1[9]) + (m0[8] * m1[10]) + (m0[12] * m1[11]);
	r[9] = (m0[1] * m1[8]) + (m0[5] * m1[9]) + (m0[9] * m1[10]) + (m0[13] * m1[11]);
	r[10] = (m0[2] * m1[8]) + (m0[6] * m1[9]) + (m0[10] * m1[10]) + (m0[14] * m1[11]);
	r[11] = (m0[3] * m1[8]) + (m0[7] * m1[9]) + (m0[11] * m1[10]) + (m0[15] * m1[11]);

	r[12] = (m0[0] * m1[12]) + (m0[4] * m1[13]) + (m0[8] * m1[14]) + (m0[12] * m1[15]);
	r[13] = (m0[1] * m1[12]) + (m0[5] * m1[13]) + (m0[9] * m1[14]) + (m0[13] * m1[15]);
	r[14] = (m0[2] * m1[12]) + (m0[6] * m1[13]) + (m0[10] * m1[14]) + (m0[14] * m1[15]);
	r[15] = (m0[3] * m1[12]) + (m0[7] * m1[13]) + (m0[11] * m1[14]) + (m0[15] * m1[15]);

	// copy result
	result[0] = r[0];
	result[1] = r[1];
	result[2] = r[2];
	result[3] = r[3];
	result[4] = r[4];
	result[5] = r[5];
	result[6] = r[6];
	result[7] = r[7];
	result[8] = r[8];
	result[9] = r[9];
	result[10] = r[10];
	result[11] = r[11];
	result[12] = r[12];
	result[13] = r[13];
	result[14] = r[14];
	result[15] = r[15];
}


internal void M4x4_SetProjection(f32 *m, f32 prjNear, f32 prjFar, f32 prjLeft, f32 prjRight, f32 prjTop, f32 prjBottom)
{
	m[0] = (2 * prjNear) / (prjRight - prjLeft);
	m[4] = 0;
	m[8] = (prjRight + prjLeft) / (prjLeft - prjRight);
	m[12] = 0;

	m[1] = 0;
	m[5] = (2 * prjNear) / (prjTop - prjBottom);
	m[9] = (prjTop + prjBottom) / (prjTop - prjBottom);
	m[13] = 0;

	m[2] = 0;
	m[6] = 0;
	m[10] = -(prjFar + prjNear) / (prjFar - prjNear);
	m[14] = (-2 * prjFar * prjNear) / (prjFar - prjNear);

	m[3] = 0;
	m[7] = 0;
	m[11] = -1;
	m[15] = 0;
}

internal void M4x4_SetOrthoProjection(f32 *m, f32 left, f32 right, f32 top, f32 bottom, f32 prjNear, f32 prjFar)
{
#if 1
	M4x4_ToIdentity(m);
	m[0] = 2 / (right - left);
	m[5] = 2 / (top - bottom);
	m[10] = -2 / (prjFar - prjNear);

	m[12] = -(right + left) / (right - left);
	m[13] = -(top + bottom) / (top - bottom);
	m[14] = -(prjFar + prjNear) / (prjFar - prjNear);
	m[15] = 1;
#endif
#if 0
    m[0] = 2; 
    m[5] = 2;
    m[10] = -0.22f;

    m[14] = -1.22f;
    m[15] = 1;
#endif
}

inline void M4x4_SetupOrthoProjection(f32 *m, f32 size, f32 aspectRatio)
{
	//M4x4_SetOrthoProjection(m, -1, 1, 1, -1, 0.1f, 20.f);
	//float size = 40;
	M4x4_SetOrthoProjection(m,
							-size * aspectRatio,
							size * aspectRatio,
							size,
							-size,
							0.1f, 60.f);
}

inline void ZE_Setup3DProjection(
	f32 *m4x4,
	i32 fov,
	f32 prjScaleFactor,
	f32 prjNear,
	f32 prjFar,
	f32 aspectRatio)
{
	if (fov <= 0)
	{
		fov = 90;
	}
	M4x4_ToIdentity(m4x4);
	f32 prjLeft = -prjScaleFactor * aspectRatio;
	f32 prjRight = prjScaleFactor * aspectRatio;
	f32 prjTop = prjScaleFactor;
	f32 prjBottom = -prjScaleFactor;

	M4x4_SetProjection(
		m4x4, prjNear, prjFar, prjLeft, prjRight, prjTop, prjBottom);
}

///////////////////////////////////////////////////////////////////////
// DATA TYPES
///////////////////////////////////////////////////////////////////////
struct Vec4
{
	f32 x, y, z, w;
};

struct ZRQuadItem
{
	Vec4 pos;
	Vec4 uvs;
	Vec4 colour;
};

struct ZRDataTexture
{
	Vec4* pixels;
	i32 width;
	i32 height;
	i32 stride;
	i32 index;
	u32 dataHandle;
	u32 diffuseHandle;

	void WriteItem(Vec4 pos, Vec4 uvs, Vec4 colour)
	{
		pixels[index++] = pos;
		pixels[index++] = uvs;
		pixels[index++] = colour;
	}
};

///////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////
ze_external zErrorCode ZE_EngineInit();

ze_external void ZE_UploadTexture(
	u8 *pixels, i32 width, i32 height, u32 *handle, i32 bDataTexture = NO);
ze_external void ZE_UploadMesh(
	i32 numVerts,
	f32* verts,
	f32* uvs,
	f32* normals,
	u32* vaoHandle,
	u32* vboHandle);
ze_external ZRDataTexture ZRGL_AllocDataTexture();
ze_external void ZR_SubmitFrame();
ze_external void ZR_DrawTest();
ze_external void ZR_DrawQuadBatch(f32* projection, f32* view, ZRDataTexture* data);


ze_external void* Platform_Alloc(zeSize size);
ze_external void* Platform_Realloc(void* ptr, zeSize size);
ze_external void Platform_Free(void* ptr);
ze_external void Platform_SwapBuffers();

ze_external f32 Window_GetAspectRatio();
ze_external f32 Window_GetMonitorRatio();

#endif // ZENGINE_H
