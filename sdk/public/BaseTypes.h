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
#else
#define DLL_EXPORT extern "C" __attribute__ ((visibility("default")))
#define DLL_IMPORT extern "C" 
#endif // WIN32

// This macro predates universal static_assert support in our toolchains
#define COMPILE_TIME_ASSERT( pred ) static_assert( pred, "Compile time assert constraint is not true: " #pred )

typedef float vec_t;

typedef unsigned char byte;

struct rgba_t
{
	byte r, g, b, a;
};

#if !defined( MAX_PATH )
#define MAX_PATH 260
#endif // !MAX_PATH

#include "PluginMath.h"

#endif // !SDK_BASETYPES_H
