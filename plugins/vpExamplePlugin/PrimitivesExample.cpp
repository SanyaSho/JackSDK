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

#include "vstdlib_static.h"

/*
===============
Primitive_CreateBlock
===============
*/
static bool Jack_Primitive_CreateBlock( const stPRIMITIVECREATESTRUCT *primCreate )
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

	assert( worldSpawn->m_editorFlags & FL_WORLDSPAWN );

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

	if ( ( primCreate->m_flags & PRIMFLAG_FLOORSNAP ) != 0 )
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
Primitive_CreateCylinder
===============
*/
static bool Jack_Primitive_CreateCylinder( const stPRIMITIVECREATESTRUCT *primCreate )
{
	int numFaces = primCreate->m_numFaces;
	if ( numFaces <= 2 )
		return false;

	qEntity_s *worldSpawn = primCreate->m_world->m_entityList;
	if ( !worldSpawn )
		return false;

	assert( worldSpawn->m_editorFlags & FL_WORLDSPAWN );

	vec3_t center = ( primCreate->m_startPos + primCreate->m_endPos ) * 0.5f;

	int uA = primCreate->m_uAxis;
	int vA = primCreate->m_vAxis;
	int hA = primCreate->m_heightAxis;

	vec2_t endUV = vec2_t( primCreate->m_endPos[uA], primCreate->m_endPos[vA] );
	vec2_t startUV = vec2_t( primCreate->m_startPos[uA], primCreate->m_startPos[vA] );

	vec2_t radius = ( endUV - startUV ) * 0.5f;

	float topHeight = primCreate->m_endPos[hA];
	float bottomHeight = primCreate->m_startPos[hA];

	struct Heights_t
	{
		vec3_t top;
		vec3_t bottom;
	};

	Heights_t *heights = (Heights_t *)Sys_Malloc( sizeof( Heights_t ) * numFaces );
	if ( !heights )
		return false;

	// Calculate heights
	for ( int i = 0; i < numFaces; i++ )
	{
		float angle = ( 2.f * M_PI * i ) / numFaces;

		if ( ( primCreate->m_flags & PRIMFLAG_CLOCKWISE ) == 0 )
		{
			angle = -angle;
		}

		float s, c;
		V_SinCosPrecise( &s, &c, angle );


		// Top ring
		heights[i].top[uA] = center[uA] + radius.x * c;
		heights[i].top[vA] = center[vA] + radius.y * s;
		heights[i].top[hA] = topHeight;

		if ( ( primCreate->m_flags & PRIMFLAG_FLOORSNAP ) != 0 )
		{
			for ( int j = 0; j < 3; j++ )
			{
				heights[i].top[j] = floorf( heights[i].top[j] + 0.5f );
			}
		}


		// Bottom ring
		heights[i].bottom[uA] = heights[i].top[uA];
		heights[i].bottom[vA] = heights[i].top[vA];
		heights[i].bottom[hA] = bottomHeight;

		if ( ( primCreate->m_flags & PRIMFLAG_FLOORSNAP ) != 0 )
		{
			// Dont need to snap U and V again
			heights[i].bottom[hA] = floorf( heights[i].bottom[hA] + 0.5f );
		}
	}

	for ( int i = 0; i < numFaces; i++ )
	{
		Sys_SnapVertex( heights[i].top.Base() );
		Sys_SnapVertex( heights[i].bottom.Base() );
	}

	qBrush_t *brushDef = Brush_Create( primCreate->m_world, worldSpawn );
	if ( !brushDef )
	{
		Sys_Free( heights );
		return false;
	}

	// Top face
	qFace_t *faceDef = Face_Create( primCreate->m_world, brushDef, &primCreate->m_texDef, numFaces );
	for ( int i = 0; i < numFaces; i++ )
	{
		faceDef->m_vertices[i].coords = heights[i].top;
	}

	// Bottom face
	faceDef = Face_Create( primCreate->m_world, brushDef, &primCreate->m_texDef, numFaces );
	for ( int i = 0; i < numFaces; i++ )
	{
		faceDef->m_vertices[i].coords = heights[numFaces - 1 - i].bottom;
	}

	// Side faces
	for ( int i = 0; i < numFaces; i++ )
	{
		faceDef = Face_Create( primCreate->m_world, brushDef, &primCreate->m_texDef, 4 );

		faceDef->m_vertices[0].coords = heights[i].top;
		faceDef->m_vertices[1].coords = heights[i].bottom;

		int next = ( i + 1 ) % numFaces;
		faceDef->m_vertices[2].coords = heights[next].bottom;
		faceDef->m_vertices[3].coords = heights[next].top;
	}

	Entity_Build( worldSpawn, ENT_BLDFLG_BIT1 | ENT_BLDFLG_BIT3 );

	Undo_Start( primCreate->m_world, "Create Cylinder" );
	Undo_AddBrush( brushDef );
	Undo_End( primCreate->m_world );

	Sys_Free( heights );

	return true;
}
primitiveDesc_t cylinderPrimitive = { "cylinder (Custom)", "(primitives)", 8, 3, 128, 0, Jack_Primitive_CreateCylinder };

// clang-format off
static primitiveDesc_t s_primList[] =
{
	blockPrimitive,				// block (Custom)
	cylinderPrimitive			// cylinder (Custom)
};
// clang-format on

/*
===============
vpEnumPrimitives
===============
*/
DLL_EXPORT int vpEnumPrimitives( pfnRegisterPrimitive registerPrimitive, void *libraryHandle )
{
	int i = 0;

	for ( i = 0; i < ARRAYSIZE( s_primList ); i++ )
	{
		registerPrimitive( &s_primList[i], libraryHandle );
	}

	return i;
}
