//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( SDK_BASETYPES_H )
#define SDK_BASETYPES_H

#include <string.h>

#if defined( WIN32 )
#define DLL_EXPORT extern "C" __declspec( dllexport )
#define DLL_IMPORT extern "C" __declspec( dllimport )

typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#define DLL_EXPORT extern "C" __attribute__ ((visibility("default")))
#define DLL_IMPORT extern "C" 

typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
#endif // WIN32

// This macro predates universal static_assert support in our toolchains
#define COMPILE_TIME_ASSERT( pred ) static_assert( pred, "Compile time assert constraint is not true: " #pred )

#if !defined( ARRAYSIZE )
#define ARRAYSIZE( p ) ( sizeof( p ) / sizeof( p[0] ) )
#endif // !ARRAYSIZE

typedef float vec_t;

typedef unsigned char byte;

struct rgba_t
{
	byte r, g, b, a;

	// clang-format off
	inline byte *data() { return (byte *)this; }
	inline const byte *data() const { return (const byte *)this; }
	// clang-format on
};

#if !defined( MAX_PATH )
#define MAX_PATH 260
#endif // !MAX_PATH

#include "InternalStructSizeOfs.h"

#include "PluginMath.h"

#endif // !SDK_BASETYPES_H
