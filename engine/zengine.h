#ifndef ZENGINE_H
#define ZENGINE_H

#include <stdint.h>	// for types
#include <stdio.h>	// for printf
#include <math.h>
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
#define ZERROR_CODE_OUT_OF_BOUNDS 4
#define ZERROR_CODE_NOT_FOUND 5
#define ZERROR_CODE_ALLOCATION_FAILED 5

#define ZSENTINEL_INT 0xDEADBEEF

//////////////////////////////////////////////////////////////////////
// Input codes
//////////////////////////////////////////////////////////////////////
#define Z_INPUT_CODE_NULL 0
#define Z_INPUT_CODE_MOUSE_1 1
#define Z_INPUT_CODE_MOUSE_2 2
#define Z_INPUT_CODE_MOUSE_3 3
#define Z_INPUT_CODE_MOUSE_4 4
#define Z_INPUT_CODE_MOUSE_5 5
#define Z_INPUT_CODE_MWHEELUP 6
#define Z_INPUT_CODE_MWHEELDOWN 7
#define Z_INPUT_CODE_A 8
#define Z_INPUT_CODE_B 9
#define Z_INPUT_CODE_C 10
#define Z_INPUT_CODE_D 11
#define Z_INPUT_CODE_E 12
#define Z_INPUT_CODE_F 13
#define Z_INPUT_CODE_G 14
#define Z_INPUT_CODE_H 15
#define Z_INPUT_CODE_I 16
#define Z_INPUT_CODE_J 17
#define Z_INPUT_CODE_K 18
#define Z_INPUT_CODE_L 19
#define Z_INPUT_CODE_M 20
#define Z_INPUT_CODE_N 21
#define Z_INPUT_CODE_O 22
#define Z_INPUT_CODE_P 23
#define Z_INPUT_CODE_Q 24
#define Z_INPUT_CODE_R 25
#define Z_INPUT_CODE_S 26
#define Z_INPUT_CODE_T 27
#define Z_INPUT_CODE_U 28
#define Z_INPUT_CODE_V 29
#define Z_INPUT_CODE_W 30
#define Z_INPUT_CODE_X 31
#define Z_INPUT_CODE_Y 32
#define Z_INPUT_CODE_Z 33
#define Z_INPUT_CODE_SPACE 34
#define Z_INPUT_CODE_LEFT_SHIFT 35
#define Z_INPUT_CODE_RIGHT_SHIFT 36
#define Z_INPUT_CODE_LEFT_CONTROL 37
#define Z_INPUT_CODE_RIGHT_CONTROL 38
#define Z_INPUT_CODE_ESCAPE 39
#define Z_INPUT_CODE_RETURN 40
#define Z_INPUT_CODE_ENTER 41
#define Z_INPUT_CODE_0 42
#define Z_INPUT_CODE_1 43
#define Z_INPUT_CODE_2 44
#define Z_INPUT_CODE_3 45
#define Z_INPUT_CODE_4 46
#define Z_INPUT_CODE_5 47
#define Z_INPUT_CODE_6 48
#define Z_INPUT_CODE_7 49
#define Z_INPUT_CODE_8 50
#define Z_INPUT_CODE_9 51
#define Z_INPUT_CODE_UP 52
#define Z_INPUT_CODE_DOWN 53
#define Z_INPUT_CODE_LEFT 54
#define Z_INPUT_CODE_RIGHT 55
#define Z_INPUT_CODE_MOUSE_POS_X 56
#define Z_INPUT_CODE_MOUSE_POS_Y 57
#define Z_INPUT_CODE_MOUSE_MOVE_X 58
#define Z_INPUT_CODE_MOUSE_MOVE_Y 59
#define Z_INPUT_CODE_F1 60
#define Z_INPUT_CODE_F2 61
#define Z_INPUT_CODE_F3 62
#define Z_INPUT_CODE_F4 63
#define Z_INPUT_CODE_F5 64
#define Z_INPUT_CODE_F6 65
#define Z_INPUT_CODE_F7 66
#define Z_INPUT_CODE_F8 67
#define Z_INPUT_CODE_F9 68
#define Z_INPUT_CODE_F10 69
#define Z_INPUT_CODE_F11 70
#define Z_INPUT_CODE_F12 71
#define Z_INPUT_CODE_BACKSLASH 72
#define Z_INPUT_CODE_FORWARDSLASH 73
#define Z_INPUT_CODE_DELETE 74

#define Z_INPUT_CODE__LAST__ 74

#define ZE_ASSERT(statement, message) \
if (!(statement)) \
{ Platform_ErrorBox(message); }


#ifndef ZE_SET_ZERO
#define ZE_SET_ZERO(ptrToMemory, numberOfBytesToZero) \
	memset(##ptrToMemory, 0, numberOfBytesToZero##);
#endif


typedef void* (*ZE_mallocFunction)(const zeSize numBytes);
typedef void (*ZE_freeFunction)(const void * memory);

#define SPECIAL_APP_MODE_NONE 0
#define SPECIAL_APP_MODE_SINGLE_SHOT 1

///////////////////////////////////////////////////////////
// Embedded assets
///////////////////////////////////////////////////////////
#if 0
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
#endif
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


inline void M4x4_SetProjection(f32 *m, f32 prjNear, f32 prjFar, f32 prjLeft, f32 prjRight, f32 prjTop, f32 prjBottom)
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

inline void M4x4_SetOrthoProjection(f32 *m, f32 left, f32 right, f32 top, f32 bottom, f32 prjNear, f32 prjFar)
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

struct ZEMemBlock
{
	i32 tag; // identifier, bit field, whatever
	i32 sentinel; // memcheck, should always == ZSENTINEL_INT
	i64 size; // jump to next block
};

struct ZEMemBlockReader
{
	i8* start;
	i64 size;
	i64 cursor;
};

struct Vec2
{
	f32 x, y;
};

struct Vec3
{
	f32 x, y, z;
};

struct Vec4
{
	f32 x, y, z, w;
};

struct Point2
{
	i32 x, y;
};

struct ZRQuadItem
{
	Vec4 pos;
	Vec4 scale;
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

	void WriteItem(Vec4 pos, Vec4 scale, Vec4 uvs, Vec4 colour)
	{
		pixels[index++] = pos;
		pixels[index++] = uvs;
		pixels[index++] = scale;
		pixels[index++] = colour;
	}

	void Clear()
	{
		index = 0;
	}
};

struct ZEWindowInfo
{
	i32 width, height;
	f32 aspectRatio;
	
	i32 scrWidth, scrHeight;
	f32 scrAspectRatio;
};

#define APP_FLAG_OVERRIDE_ESCAPE_KEY (1 << 0)

struct ZEFrame
{
	f32 delta;
};

typedef void (ZE_FrameCallback)(ZEFrame frame);
typedef void (ZE_KeyCallback)(i32 key, i32 value, i32 mods);
typedef void (*ZE_ConsoleCallback)(char* fullString, char** tokens, i32 numTokens);

#define ZCMD_CALLBACK(functionName) \
internal void functionName(char* fullString, char** tokens, i32 numTokens)

struct ZEApp
{
	char* windowName;
	i32 flags;
	ZE_FrameCallback* frameCallback;
	ZE_KeyCallback* keyCallback;
};

///////////////////////////////////////////////////////////////////////
// INTERNAL FUNCTIONS
///////////////////////////////////////////////////////////////////////

inline zeSize ZE_Copy(void *dest, void *source, zeSize numBytes)
{
	memcpy(dest, source, numBytes);
	return numBytes;
}

inline void* ZE_PtrFromOffset(void* start, zeSize offset)
{
	return ((u8*)start + offset);
}

static i32 ZStr_LenNoTerminator(const char* str)
{
    i32 count = 0;
    while (str[count]) { ++count; }
    return count;
}

static i32 ZStr_Len(const char* str)
{
    i32 count = 0;
    while (str[count]) { ++count; }
    // include terminator
    return count + 1;
}

internal i32 ZStr_Equal(const char *a, const char *b)
{
    if (a == NULL || b == NULL)
    {
        return (a == b);
    }
    while (*a == *b)
    {
        if (*a == '\0' || *b == '\0')
        {
            return (*a == *b);
        }
        ++a;
        ++b;
    }
    return NO;
}

/**
 * Returns chars written
 * Copy a string without exceeding the specified limit
 * Will always patch a NULL terminator at the final position
 */
internal i32 ZStr_CopyLimited(const char *source, char *target, zeSize limit)
{
    if (limit < 1) { return 0; }
    i32 written = 0;
    char* targetStart = target;
    while (*source && limit > 0)
    {
        *target++ = *source++;
        --limit;
        ++written;
    }
	*target = '\0';
    return ++written;
}

// decimal or hexadecimal
// negative and positive
// "-54" "12" "0x432146fd" "-0X4AbdC"
internal i32 ZStr_AsciToInt32(const char *str)
{
    i32 sign = 1;
    i32 val = 0;
    char c;
    if (*str == '-')
    {
        sign = -1;
        ++str;
    }

    // hexadecimal
    if (*str == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        // Move past first two characters
        str += 2;
        while (1)
        {
            c = *str;
            ++str;
            if (c >= '0' && c <= '9')
            {
                val = val * 16 + c - '0';
            }
            else if (c >= 'a' && c <= 'f')
            {
                val = val * 16 + c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F')
            {
                val = val * 16 + c - 'A' + 10;
            }
            else
            {
                return val * sign;
            }
        }
    }

    // decimal
    while (true)
    {
        c = *str;
        ++str;
        if (c < '0' || c > '9')
        {
            // no numerical character
            return sign * val;
        }
        // '0' char = 48 in asci, so
        // c minus '0' = c minus 48
        // val * 10 moves to next digit
        // then add new value
        val = (val * 10) + (c - '0');
    }
    return val * sign;
}


/**
 * Returns 1 if there is still text remaining
 */
internal i32 ZStr_ReadLine(char** source, char* target, zeSize capacity)
{
    i32 i = 0;
    char* cursor = *source;
    for(;;)
    {
        char c = cursor[i];
        // end of line or buffer, but not end of text
        if (c == '\n')
        {
            *source = &cursor[i] + 1;
            target[i] = '\0';
            // printf("End line at %d on code %d\n", i, c);
            return YES;
        }
        // end of text
        else if (c == '\0')
        {
            *source = cursor + 1;
            target[i] = '\0';
            // printf("End line at %d on code %d\n", i, c);
            return NO;
        }
        // carriage return. replace it with null but continue to 
        // make sure the newline is consumed as well.
        if (c == '\r')
        {
            c = ' ';
        }
        target[i] = c;
        i += 1;
    }
}

internal i32 ZStr_FindFirstCharMatch(char* start, char match)
{
    i32 i = 0;
    for(;;)
    {
        char c = start[i];
        if (c == match) { return i; }
        if (c == '\0') { break; }
        i++;
    }
    return -1;
}

internal Point2 ZStr_FindToken(const char* str, i32 tokenIndex, char separator)
{
    Point2 p = {};
    i32 currentTokenIndex = 0;
    i32 i = 0;
    i32 bInToken = NO;
    char c;
    while ((c = str[i++]) != '\0')
    {
        if (bInToken)
        {
            if (c == separator)
            {
                bInToken = NO;
                if (currentTokenIndex == tokenIndex)
                {
                    p.y = i - 1;
                    return p;
                }
                currentTokenIndex += 1;
            }
            continue;
        }
        else
        {
            if (c != separator)
            {
                bInToken = YES;
                if (currentTokenIndex == tokenIndex)
                {
                    p.x = i - 1;
                }
            }
        }
    }
    return p;
}

/**
 * Find string tokens:
 * > Copies source into dest, placing '\0' at the end of every token
 * > Destination must be at least as long as source!
 * > Returns number of tokens found, up to the max allowed by
 * the passed in results array
 * > Should be okay if destination and token list memory is not cleared.
 * > Send same pointer to source and destination to tokenise in-place.
 */
internal i32 ZStr_Tokenise(const char* source, char* destination, char** tokens, i32 maxTokens)
{
    i32 len = ZStr_Len(source);
    i32 tokensCount = 0;

    i32 readPos = 0;
    i32 writePos = 0;

    u8 reading = true;
    u8 readingToken = 0;
    while (reading)
    {
        char c = *(source + readPos);
        if (readingToken)
        {
            if (c == ' ')
            {
				// finish token
                *(destination + writePos) = '\0';
				readingToken = 0;
            }
            else if (c == '\0')
            {
				// finish token and complete string
                *(destination + writePos) = '\0';
				readingToken = 0;
                reading = false;
            }
            else
            {
                *(destination + writePos) = c;
            }
            readPos++;
            writePos++;

			// Check if finished and results are full
			if (readingToken == 0 && tokensCount >= maxTokens)
			{
				reading = false;
			}
        }
        else
        {
			// probe forward for a token start
            if (c == ' ' || c == '\t')
            {
                readPos++;
            }
            else if (c == '\0')
            {
                *(destination + writePos) = '\0';
                reading = false;
            }
            else
            {
				// Start token
                readingToken = 1;
                *(destination + writePos) = c;

                tokens[tokensCount++] = (destination + writePos);

                readPos++;
                writePos++;
            }
        }
    }

    return tokensCount;
}

ze_internal Point2 ZE_MeasureTextBlock(char* text)
{
	Point2 p = {};
	p.x = 0;
	p.y = 0;
	
	i32 curLen = 0;
	i32 i = 0;
	char c = text[i];
	while (c != '\0')
	{
		if (c == '\n')
		{
			p.y += 1;
			if (curLen > p.x)
			{
				p.x = curLen;
				curLen = 0;
			}
		}
		else
		{
			curLen += 1;
		}
		i++;
		c = text[i];
	}

	return p;
}

// Count chars to end of line/text
ze_internal i32 ZE_MeasureLineLength(char* text)
{
	i32 i = 0;
	char c = text[i];
	while (c != '\0' && c != '\n')
	{
		i++;
		c = text[i];
	}
	return i;
}

///////////////////////////////////////////////////////////////////////
// EXPORTED FUNCTIONS
///////////////////////////////////////////////////////////////////////

ze_external void			Platform_SetCursorLock(i32 bLocked);
ze_external void			Platform_Sleep(i32 milliseconds);
ze_external f64				Platform_QueryClock();
ze_external void			Platform_ErrorBox(char* message);

ze_external void* 			Platform_Alloc(zeSize size);
ze_external void* 			Platform_Realloc(void* ptr, zeSize size);
ze_external void 			Platform_Free(void* ptr);
ze_external void 			Platform_SwapBuffers();
ze_external ZEWindowInfo	Platform_GetWindowInfo();

ze_external i32             ZE_FindCommandLineIndex(char* shortName, char* longName);
ze_external i32             ZE_GetCommandLineArgCount();
ze_external char*           ZE_GetCommandLineArg(i32 index);

ze_external void			Platform_Screenshot(const char* filePath);
ze_external void 			ZEPlatform_SaveImageRGB(
	const char *fileName, i32 width, i32 height, const void *rgbPixels);
ze_external void 			ZEPlatform_SaveImageRGBA(
	const char *fileName, i32 width, i32 height, const void *rgbaPixels);

ze_external zErrorCode		ZE_EngineStart(ZEApp app, i32 argc, char** argv);
ze_external i32				ZEGetSpecialMode();
ze_external zErrorCode		ZERunLoop(i32 targetFrameRate, ZE_FrameCallback frameCallback);
ze_external void			Platform_Shutdown();

ze_external zErrorCode		ZE_RegisterConsoleCommand(char *name, char *description, ZE_ConsoleCallback callback);
ze_external void			ZE_QueueConsoleCommand(char* cmd);

ze_external void 			ZR_UploadTexture(void *pixels, i32 width, i32 height, u32 *handle, i32 bDataTexture);
ze_external void			ZR_UploadMesh(i32 numVerts, f32* verts, f32* uvs, f32* normals, u32* vaoHandle, u32* vboHandle);
ze_external ZRDataTexture	ZR_AllocDataTexture();
ze_external void 			ZR_BeginFrame(f32 clearRed, f32 clearGreen, f32 clearBlue);
ze_external void 			ZR_DrawTest();
ze_external void 			ZR_DrawQuadBatch(f32* projection, f32* view, ZRDataTexture* data);
ze_external void 			ZR_SubmitFrame();
ze_external void 			ZR_GetAsciiUVs(char ascii, f32* minX, f32* minY, f32* maxX, f32* maxY);

#endif // ZENGINE_H
