//=====================================================================================//
//
// Purpose: J.A.C.K. Example Plugin
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <assert.h>

#include "PluginAPI.h"
#include "PluginEntity.h"
#include "PluginPrimitives.h"
#include "PluginWorld.h"

/*
===============
Primitive_CreateBlock
===============
*/
bool Jack_Primitive_CreateBlock( const stPRIMITIVECREATESTRUCT *primCreate )
{
	// clang-format off
	static unsigned int faceTable[6][4] = {
		{ 0, 1, 3, 2 },
		{ 2, 3, 5, 4 },
		{ 4, 5, 7, 6 },
		{ 6, 7, 1, 0 },
		{ 1, 7, 5, 3 },
		{ 0, 2, 4, 6 }
	};
	// clang-format on

	vec3_t vertexTable[8];
	memset( vertexTable, 0, sizeof( vertexTable ) );

	qEntity_s *worldSpawn = primCreate->m_world->m_entityList;
	if ( !worldSpawn )
		return false;

	assert( worldSpawn->m_editorFlags & 0x20 );

	float startX = primCreate->m_startPos.x;
	float startY = primCreate->m_startPos.y;
	float startZ = primCreate->m_startPos.z;

	float endX = primCreate->m_endPos.x;
	float endY = primCreate->m_endPos.y;
	float endZ = primCreate->m_endPos.z;

	// clang-format off
	vertexTable[0] = { startX, startY, startZ };
	vertexTable[1] = { startX, startY, endZ   };

	vertexTable[2] = { startX, endY,   startZ };
	vertexTable[3] = { startX, endY,   endZ   };

	vertexTable[4] = { endX,   endY,   startZ };
	vertexTable[5] = { endX,   endY,   endZ   };

	vertexTable[6] = { endX,   startY, startZ };
	vertexTable[7] = { endX,   startY, endZ   };
	// clang-format on

	if ( ( primCreate->m_flags & PRIMFLAG_FLOOR ) != 0 )
	{
		for ( int i = 0; i < 8; i++ )
		{
			vertexTable[i].x = floorf( vertexTable[i].x + 0.5f );
			vertexTable[i].y = floorf( vertexTable[i].y + 0.5f );
			vertexTable[i].z = floorf( vertexTable[i].z + 0.5f );
		}
	}

	for ( int i = 0; i < 8; i++ )
	{
		Sys_SnapVertex( vertexTable[i].Base() );
	}

	qBrush_s *brushDef = Brush_Create( primCreate->m_world, worldSpawn );
	if ( !brushDef )
		return false;

	for ( int i = 0; i < 6; i++ )
	{
		qFace_s *faceDef = Face_Create( primCreate->m_world, brushDef, &primCreate->m_texDef, 4 );
		if ( !faceDef )
			return false;

		for ( int j = 0; j < 4; j++ )
		{
			faceDef->m_vertices[j].coords = vertexTable[faceTable[i][j]];
		}
	}

	Entity_Build( worldSpawn, ENT_BLDFLG_BIT1 | ENT_BLDFLG_BIT3 );

	Undo_Start( primCreate->m_world, "Create Block" );
	Undo_AddBrush( brushDef );
	Undo_End( primCreate->m_world );

	return true;
}
primitiveDesc_t blockPrimitive = { "block (Custom)", "(primitives)", 0, 0, 0, 0, Jack_Primitive_CreateBlock };

/*
===============
vpEnumPrimitives
===============
*/
DLL_EXPORT int vpEnumPrimitives( pfnRegisterPrimitive registerPrimitive, void *libraryHandle )
{
	registerPrimitive( &blockPrimitive, libraryHandle );
	return 1;
}
