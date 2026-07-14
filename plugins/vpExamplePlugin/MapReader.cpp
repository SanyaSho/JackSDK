//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>

#include "MapReader.h"

// Plugin API
#include "PluginAPI.h"
#include "PluginEntity.h"
#include "PluginRender.h"
#include "PluginFaces.h"

inline int ReadInt( bool nextLine = false )
{
	SC_GetToken( nextLine );
	return atoi( SC_Token() );
}

inline float ReadFloat( bool nextLine = false )
{
	SC_GetToken( nextLine );
	return atof( SC_Token() );
}

inline vec2_t ReadVector2D( bool nextLine = false )
{
	vec2_t vec;

	SC_GetToken( nextLine );
	vec.x = atof( SC_Token() );
	SC_GetToken( nextLine );
	vec.y = atof( SC_Token() );

	return vec;
}

inline vec3_t ReadVector3D( bool nextLine = false )
{
	vec3_t vec;

	SC_GetToken( nextLine );
	vec.x = atof( SC_Token() );
	SC_GetToken( nextLine );
	vec.y = atof( SC_Token() );
	SC_GetToken( nextLine );
	vec.z = atof( SC_Token() );

	return vec;
}

MapReader::MapReader( const char *filePath, long seekOffset, long readLimit, qWorld_s *worldDef )
	: m_seekOffset( seekOffset ), m_readLimit( readLimit ), m_invalidSolids( 0 ), m_world( worldDef ), m_mapVersion( 0 )
{
	strncpy( m_filePath, filePath, sizeof( m_filePath ) );
	m_filePath[sizeof( m_filePath ) - 1] = '\0';
}

MapReader::~MapReader()
{
}

bool MapReader::LoadMap()
{
	Sys_Printf( "Loading: \"%s\"", m_filePath );

	if ( SC_ParseFromFile( m_filePath, m_seekOffset, m_readLimit, 0 ) )
	{
		while ( SC_GetToken( true ) )
		{
			SC_UnGetToken();
			ParseMap();
		}

		if ( m_invalidSolids != 0 )
		{
			Sys_Error( "%i solids were not loaded due to errors in MAP file", m_invalidSolids );
		}

		return true;
	}
	else
	{
		char szErrorStr[256] = { 0 };
		strncpy( szErrorStr, strerror( errno ), sizeof( szErrorStr ) );
		Sys_Error( "can't read \"%s\" (%s)", m_filePath, szErrorStr );

		return false;
	}

	return false;
}

bool MapReader::ParseMap()
{
	epair_s *epairs = nullptr;
	epair_s *lastEpair = nullptr;

	qEntity_s *entity = nullptr;

	char classname[512] = {};
	char key[96];
	char value[512];

	float origin[3] = {};
	float angles[3] = {};

	int editorFlags = 1;

	auto CreateEntity = [&]() -> bool {
		if ( entity )
			return true;

		if ( !classname[0] )
			return false;

		if ( !stricmp( classname, "worldspawn" ) )
			editorFlags |= 0x20;

		entity = Entity_Create( m_world, classname, origin, editorFlags );
		if ( !entity )
			return false;

		entity->epairs = epairs;

		entity->m_vecAngles = *(vec3_t *)angles;

		return true;
	};

	SC_MatchToken( "{" );

	if ( SC_CheckError() )
		return false;

	while ( SC_GetToken( true ) )
	{
		if ( !strcmp( SC_Token(), "}" ) )
			break;

		if ( !strcmp( SC_Token(), "{" ) )
		{
			editorFlags &= ~1;

			if ( !CreateEntity() )
			{
				FreeEpairList( epairs );
				return false;
			}

			SC_UnGetToken();

			if ( !ParseBrushes( entity ) )
			{
				++m_invalidSolids;

				SC_ResetError();

				while ( SC_GetToken( true ) )
				{
					if ( !strcmp( SC_Token(), "}" ) )
						break;
				}
			}

			continue;
		}

		if ( entity )
		{
			SC_ParseError( "epairs are not allowed after brushes" );

			SC_ResetError();

			while ( SC_GetToken( true ) )
			{
				if ( !strcmp( SC_Token(), "}" ) )
					break;
			}

			SC_UnGetToken();

			continue;
		}

		//
		// key
		//
		strcpy( key, SC_Token() );

		SC_GetToken( false );
		if ( SC_CheckError() )
		{
			FreeEpairList( epairs );
			return false;
		}

		//
		// value
		//
		strncpy( value, SC_Token(), sizeof( value ) );
		value[sizeof( value ) - 1] = '\0';

		if ( !stricmp( key, "classname" ) )
		{
			strcpy( classname, value );
			continue;
		}

		if ( !stricmp( key, "origin" ) )
		{
			sscanf( value, "%f %f %f", &origin[0], &origin[1], &origin[2] );
			continue;
		}

		if ( !stricmp( key, "angles" ) )
		{
			sscanf( value, "%f %f %f", &angles[0], &angles[1], &angles[2] );
		}
		else if ( !stricmp( key, "angle" ) )
		{
			float yaw = atof( value );

			if ( yaw == -1.0f )
			{
				angles[0] = -90;
				angles[1] = 0;
				angles[2] = 0;
			}
			else if ( yaw == -2.0f )
			{
				angles[0] = 90;
				angles[1] = 0;
				angles[2] = 0;
			}
			else
			{
				angles[0] = 0;
				angles[1] = yaw;
				angles[2] = 0;
			}
		}
		else if ( !stricmp( key, "mapversion" ) )
		{
			m_mapVersion = atoi( value );
			continue;
		}
		else if ( !stricmp( key, "wad" ) )
		{
			continue;
		}
		else if ( !stricmp( key, "_generator" ) )
		{
			continue;
		}

		epair_s *epair = epairs;

		while ( epair )
		{
			if ( !stricmp( epair->key, key ) )
				break;

			epair = epair->next;
		}

		if ( !epair )
		{
			epair = (epair_s *)Sys_Malloc( sizeof( epair_s ) );

			epair->key = Sys_AllocString( key );
			epair->value = Sys_AllocString( value );
			epair->next = nullptr;

			if ( lastEpair )
				lastEpair->next = epair;
			else
				epairs = epair;

			lastEpair = epair;
		}
	}

	if ( !CreateEntity() )
	{
		FreeEpairList( epairs );
		return false;
	}

	int buildFlags =
		( m_mapVersion >= 220 )
			? 5	  // 0 | 2
			: 13; // 0 | 2 | 3

	Entity_Build( entity, buildFlags );

	return true;
}

bool MapReader::ParseBrushes( qEntity_t *parentEntity )
{
	SC_MatchToken( "{" );

	if ( SC_CheckError() )
		return false;

	if ( SC_CheckError() )
		return false;

	qBrush_s *brushDef = Brush_Create( m_world, parentEntity );
	if ( !brushDef )
		return false;

	while ( SC_GetToken( true ) )
	{
		if ( !strcmp( SC_Token(), "}" ) )
			break;

		SC_UnGetToken();

		// Parse brush faces
		if ( !ParseFaces( brushDef ) )
		{
			return false;
		}
	}

	return true;
}

bool MapReader::ParseFaces( qBrush_s *brushOwner )
{
	// Quake
	// p0                p1                p2                tname    X X X X        X       
	// ( -328 1352 144 ) ( -328 1472 144 ) ( -336 1472 144 ) METAL1_3 0 0 0 1.000000 1.000000

	// Half-Life
	// p0               p1             p2              tname   [ X X X X ] [ X X  X X ] X X X 
	// ( 256 -256 160 ) ( 256 -256 0 ) ( 256 256 160 ) C1A0_WX [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1

	vec3_t p0, p1, p2;

	SC_Parse1DMatrix( 3, p0.Base() );
	SC_Parse1DMatrix( 3, p1.Base() );
	SC_Parse1DMatrix( 3, p2.Base() );

	qPlane_t plane;
	//CrossProduct( p2 - p1, p0 - p1, plane.normal ); // winbspc
	CrossProduct( p0 - p1, p2 - p0, plane.normal );
	VectorNormalize( plane.normal );

	plane.dist = DotProduct( plane.normal, p0 );

	if ( fabs( plane.normal.x ) == 1.0f )
		plane.alignedAxis = 0;
	else if ( fabs( plane.normal.y ) == 1.0f )
		plane.alignedAxis = 1;
	else if ( fabs( plane.normal.z ) == 1.0f )
		plane.alignedAxis = 2;
	else
		plane.alignedAxis = 3;

	qTexDef_t texDef;
	memset( &texDef, 0, sizeof( qTexDef_t ) );

	int flags = SC_GetParseFlags();
	SC_SetParseFlags( flags | 1 );

	SC_GetToken( false );
	strncpy( texDef.m_textureName, SC_Token(), sizeof( texDef.m_textureName ) );
	texDef.m_textureName[sizeof( texDef.m_textureName ) - 1] = '\0';

	SC_SetParseFlags( flags );

	for ( char *p = texDef.m_textureName; *p; ++p )
		*p = (char)toupper( *p );

	/*const char *block = SC_CopyBlock();
	Sys_Printf( "%s", block );
	Sys_Free( (char *)block );*/

	if ( m_mapVersion < 220 )
	{
		SC_GetToken( false );

		if ( !strcmp( SC_Token(), "[" ) )
		{
			Sys_Warning( "not a version %i file (is it a decompiled BSP output?)", m_mapVersion );

			m_mapVersion = 220;
			SC_UnGetToken();
		}
		else
		{
			SC_UnGetToken();

			texDef.m_xShift = ReadFloat();
			texDef.m_yShift = ReadFloat();

			texDef.m_rotate = -ReadFloat();

			texDef.m_scale.x = ReadFloat();
			texDef.m_scale.y = ReadFloat();

			texDef.m_textureAlignment = 4;
		}
	}

	if ( m_mapVersion >= 220 )
	{
		SC_GetToken( false );

		if ( strcmp( SC_Token(), "[" ) )
		{
			Sys_Warning( "not a version %i file (is it a decompiled BSP output?)", m_mapVersion );

			Sys_Warning( "missing '[' in texturedef at line %i ('%s')", SC_Line(), SC_Token() );

			m_mapVersion = 0;
			SC_UnGetToken();
		}

		texDef.m_UAxis = ReadVector3D();
		texDef.m_xShift = ReadFloat();

		SC_MatchToken( "]" );
		if ( SC_CheckError() )
			return false;

		SC_MatchToken( "[" );
		if ( SC_CheckError() )
			return false;

		texDef.m_VAxis = ReadVector3D();
		texDef.m_yShift = ReadFloat();

		SC_MatchToken( "]" );
		if ( SC_CheckError() )
			return false;

		texDef.m_rotate = ReadFloat();

		texDef.m_scale = ReadVector2D();

		texDef.m_textureAlignment = 2;
	}

	while ( texDef.m_rotate < 0.0f )
		texDef.m_rotate += 360.0f;

	while ( texDef.m_rotate >= 360.0f )
		texDef.m_rotate -= 360.0f;

	if ( texDef.m_scale.x == 0.0f )
		texDef.m_scale.x = 1.0f;

	if ( texDef.m_scale.y == 0.0f )
		texDef.m_scale.y = 1.0f;

	// Skip everything else
	while ( SC_TokenAvailable() )
		SC_GetToken( false );

	qFace_s *face = Face_Create( m_world, brushOwner, &texDef, 0 );

	if ( !face )
		return false;

	face->m_plane = plane;

	return true;
}
