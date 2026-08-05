//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// Versions: "J.A.C.K. 1.1.3773 Freeware"; "J.A.C.K. 1.1.3773"; "J.A.C.K. 1.2.4603"
//
// Copyright (C) 2026  SanyaSho
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <https://www.gnu.org/licenses/>.
//
//=====================================================================================//

#if !defined( SDK_BASETYPES_H )
#define SDK_BASETYPES_H

// clang-format off

/*
 Use this define to select which API level should be implemented.

 10   - HLFX   Freeware
 100  - Steam  Public
 121  - Steam  Beta
*/
#define API_VERSION_HLFX_FREEWARE	10		/* Has the initial versions of the API structs */
#define API_VERSION_STEAM_PUBLIC	100		/* Same as API_VERSION_HLFX_FREEWARE, but some of API structs and functions were mixed or swapped */
#define API_VERSION_STEAM_BETA		121		/* API_VERSION_STEAM_PUBLIC + New functions from the Beta */

#if !defined( JACK_API_VERSION )
#define JACK_API_VERSION			API_VERSION_STEAM_BETA
#endif // !JACK_API_VERSION

// clang-format on

#include <string.h>

#if defined( WIN32 )
#define DLL_EXPORT extern "C" __declspec( dllexport )
#define DLL_IMPORT extern "C" __declspec( dllimport )

#define FORCEINLINE __forceinline

typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#define DLL_EXPORT extern "C" __attribute__ ((visibility("default")))
#define DLL_IMPORT extern "C" 

#define FORCEINLINE inline __attribute__((always_inline))

typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

#define stricmp strcasecmp
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

	rgba_t() { r = g = b = a = 0; }
	rgba_t( byte r, byte g, byte b, byte a ) { this->r = r; this->g = g; this->b = b; this->a = a; }

	// clang-format off
	FORCEINLINE byte *data() { return (byte *)this; }
	FORCEINLINE const byte *data() const { return (const byte *)this; }
	// clang-format on
};

#if !defined( MAX_PATH )
#if defined( WIN32 )
#define MAX_PATH 260 // _MAX_PATH
#else
#define MAX_PATH 1024 // _XOPEN_PATH_MAX
#endif
#endif // !MAX_PATH

#include "InternalStructSizeOfs.h"

#include "PluginMath.h"

#define FBitSet( bitlist, bits )	( bitlist &   ( bits ) )
#define SetBits( bitlist, bits )	( bitlist |=  ( bits ) )
#define ClearBits( bitlist, bits )	( bitlist &= ~( bits ) )

#endif // !SDK_BASETYPES_H
