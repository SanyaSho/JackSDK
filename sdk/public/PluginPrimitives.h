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

#if !defined( PLUGINPRIMITIVES_H )
#define PLUGINPRIMITIVES_H

/*
 Primitives API

 Primitives are registered just like the plugin actions.
 
 Example:
 primitiveDesc_t customPrimitive = { "custom", "(primitives)", 0, 0, 0, 0, []( stPRIMITIVECREATESTRUCT * ) { return false; } };
 
 DLL_EXPORT int vpEnumPrimitives( pfnRegisterPrimitive registerPrimitive, void *libraryHandle )
 {
 	registerPrimitive( &customPrimitive, libraryHandle );
 	return 1;
 }

 Retured value is the count of primitives added by the plugin.
 libraryHandle must be redirected into the registerPrimitive call.
*/

#include "BaseTypes.h"

// Need to include PluginFaces.h here becase of qTexDef_s usage below
#include "PluginFaces.h"

struct qWorld_s;
//struct qTexDef_s;


struct stPRIMITIVECREATESTRUCT;

#define PRIMITIVEDESC_FLAG_CURVE		( 1 << 6 ) /* The primitive will be visible only if the gameprofile supports curves */

typedef struct primitiveDesc_s
{
	/* Primitive name. Will be set to "?unknown?" if empty */
	const char *m_primitiveName;

	/* Primitive category. Will be set to "(unknown)" if empty */
	const char *m_primitiveCategory;

	/* Count of faces used by default. Can be 0 */
	int m_numFaces;

	/* Minimal count of faces required to spawn this primitive. Can be 0 */
	int m_minFaces;

	/* Maximum possible faces used by this primitive. Can be 0 */
	int m_maxFaces;

	/* Flags (see defines above) */
	int m_flags;

	/* Function called when primitive is being created. Function must return false if something has failed and return true on success  */
	bool (*pfnDispatchFunc)( const stPRIMITIVECREATESTRUCT *primCreate );
} primitiveDesc_t;
COMPILE_TIME_ASSERT( sizeof( primitiveDesc_t ) == SIZEOF_PRIMITIVEDESC_S );


#define PRIMFLAG_CLOCKWISE ( 1 << 0 )
#define PRIMFLAG_FLOORSNAP ( 1 << 1 )

struct stPRIMITIVECREATESTRUCT
{
	/* World this primitive belongs to */
	struct qWorld_s *m_world;

	/* Texture information used to create this primitive */
	struct qTexDef_s m_texDef;

	/* Count of faces that this primitive must have */
	int m_numFaces;

	/* Flags (see defines above) */
	int m_flags;

	int m_uAxis;
	int m_vAxis;
	int m_heightAxis;

	/* Boundaries of the primitive */
	vec3_t m_startPos;
	vec3_t m_endPos;
};
COMPILE_TIME_ASSERT( sizeof( stPRIMITIVECREATESTRUCT ) == SIZEOF_STPRIMITIVECREATESTRUCT );
// 0xB0 (184) - Linux64; 0xB8 (176) - Win64
// but even with extra 8 bytes it doesn't complain

// clang-format off

// Prototype for the function that is used to register primitives internally
typedef void (*pfnRegisterPrimitive)( const primitiveDesc_s *primDesc, void *libraryHandle );

typedef int (*vpEnumPrimitives_t)( pfnRegisterPrimitive registerPrimitive, void *libraryHandle );

// clang-format on

#endif // !PLUGINPRIMITIVES_H
