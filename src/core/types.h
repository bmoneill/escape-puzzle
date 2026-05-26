/**
 * @file core/types.h
 * @brief Common type definitions.
 */
#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <float.h>
#include <stdbool.h>

#define U8_MIN 0
#define U8_MAX 255

#define U16_MIN 0
#define U16_MAX 65535

#define U32_MIN 0
#define U32_MAX 4294967295U

#define U64_MIN 0
#define U64_MAX 18446744073709551615ULL

#define I8_MAX 127
#define I8_MIN I8_MAX * -1 - 1

#define I16_MAX 32767
#define I16_MIN I16_MAX * -1 - 1

#define I32_MAX 2147483647
#define I32_MIN I32_MAX * -1 - 1

#define I64_MAX 9223372036854775807LL
#define I64_MIN INT64_MAX * -1 - 1

#define F32_MIN FLT_MIN
#define F32_MAX FLT_MAX
#define F64_MIN DBL_MIN
#define F64_MAX DBL_MAX

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define i8  char
#define i16 short
#define i32 int
#define i64 long long

#define f32 float
#define f64 double

#ifndef NULL
#define NULL ((void*) 0)
#endif

typedef struct {
    u32 x, y;
} vec2u;

typedef struct {
    f32 x, y;
} vec2f;

typedef struct {
    u32 x, y, z;
} vec3u;

typedef struct {
    f32 x, y, z;
} vec3f;

// ==============================================================================
// Helper Macros
// ==============================================================================

#define VEC2U(x, y)    ((vec2u) { (u32) (x), (u32) (y) })
#define VEC3U(x, y, z) ((vec3u) { (u32) (x), (u32) (y), (u32) (z) })
#define VEC2F(x, y)    ((vec2f) { (f32) (x), (f32) (y) })
#define VEC3F(x, y, z) ((vec3f) { (f32) (x), (f32) (y), (f32) (z) })

#endif
