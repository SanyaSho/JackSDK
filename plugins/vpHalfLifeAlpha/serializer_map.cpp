//=====================================================================================//
//
// Purpose: Half-Life Alpha 0.52 plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

// Plugin API
#include "PluginMeta.h"

#include "DataSerializer.h"
#include "serializer_map.h"

#include "vstdlib_static.h"

#define MAPVERSION_LEGACY				0
#define MAPVERSION_VALVE220				220
#define MAPVERSION						MAPVERSION_LEGACY

//#define FINE_OUTPUT

#if defined( FINE_OUTPUT )
#define EXTRA_TAB						"\t"
#else
#define EXTRA_TAB
#endif // FINE_OUTPUT

#if defined( FINE_OUTPUT )
static int s_entCnt = 0;
#endif // FINE_OUTPUT

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
/*
===============
ExportMAP
===============
*/
bool ExportMAP( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
{
	MAPSerializer mapSerializer( filePath, seekOffset, readLimit, worldDef, FMODE_PARSERAPI );
	return mapSerializer.Export();
}

/*
===============
ImportMAP
===============
*/
bool ImportMAP( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
{
	MAPSerializer mapSerializer( filePath, seekOffset, readLimit, worldDef, FMODE_PARSERAPI );
	return mapSerializer.Import();
}
#else
/*
===============
ExportMAP
===============
*/
bool ExportMAP( const char *filePath, struct qWorld_s *worldDef )
{
	MAPSerializer mapSerializer( filePath, worldDef, FMODE_PARSERAPI );
	return mapSerializer.Export();
}

/*
===============
ImportMAP
===============
*/
bool ImportMAP( const char *filePath, struct qWorld_s *worldDef )
{
	MAPSerializer mapSerializer( filePath, worldDef, FMODE_PARSERAPI );
	return mapSerializer.Import();
}
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

/*
===============
MAPSerializer
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
MAPSerializer::MAPSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef, int fileMode )
	: Serializer( filePath, seekOffset, readLimit, worldDef, fileMode )
#else
MAPSerializer::MAPSerializer( const char *filePath, struct qWorld_s *worldDef, int fileMode )
	: Serializer( filePath, worldDef, fileMode )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
	m_mapVersion = 0;
	m_numInvalidSolid = 0;
	m_packageList = NULL;
	m_cordon = false;
}

/*
===============
~MAPSerializer
===============
*/
MAPSerializer ::~MAPSerializer()
{
#if defined( FINE_OUTPUT )
	s_entCnt = 0;
#endif // FINE_OUTPUT
}

/*
===============
Export
===============
*/
bool MAPSerializer::Export()
{
	if ( !BuildPackageList( m_worldDef, &m_packageList, ';', 1 ) )
	{
		return false;
	}

	if ( !FBitSet( m_worldDef->m_editorFlags, ( 1 << 23 ) ) )
	{
		Sys_Printf( "Saving: \"%s\"", m_filePath );
	}

	if ( !OpenForWrite() )
	{
		return false;
	}

	m_mapVersion = MAPVERSION;

	m_cordon = FBitSet( m_worldDef->m_editorFlags, EFL_CORDON );

	if ( m_cordon &&
		( fabs( m_worldDef->m_vecCordonMin.x ) < 0.001 && fabs( m_worldDef->m_vecCordonMin.y ) < 0.001 && fabs( m_worldDef->m_vecCordonMin.z ) < 0.001 ||
			fabs( m_worldDef->m_vecCordonMax.x ) < 0.001 && fabs( m_worldDef->m_vecCordonMax.y ) < 0.001 && fabs( m_worldDef->m_vecCordonMax.z ) < 0.001 ) )
	{
		m_cordon = false;
	}

	bool success = true;

	//
	// Serialize entities
	//
	for ( qEntity_t *entityDef = m_worldDef->m_entityList; entityDef != NULL; entityDef = entityDef->next )
	{
		if ( !SerializeEntities( entityDef ) )
		{
			success = false;
			break;
		}
	}

	//
	// Serialize path nodes
	//
	if ( success )
	{
		for ( qPath_t *pathDef = m_worldDef->m_pathList; pathDef != NULL; pathDef = pathDef->next )
		{
			if ( !SerializePathNodes( pathDef ) )
			{
				success = false;
				break;
			}
		}
	}

	//
	// Cleanup packages
	//
	if ( m_packageList )
	{
		Sys_Free( m_packageList );
		m_packageList = nullptr;
	}

	return success;
}

/*
===============
Import
===============
*/
bool MAPSerializer::Import()
{
	if ( !FBitSet( m_worldDef->m_editorFlags, ( 1 << 23 ) ) )
	{
		Sys_Printf( "Loading: \"%s\"", m_filePath );
	}

	if ( !OpenForRead() )
	{
		return false;
	}

	m_mapVersion = 0;

	while ( m_parser->pfnSC_GetToken( true ) )
	{
		m_parser->pfnSC_UnGetToken();

		SerializeEntities( NULL );
	}

	if ( m_numInvalidSolid != 0 )
	{
		Sys_Printf( "%i solids were not loaded due to errors in MAP file", m_numInvalidSolid );
	}

	return true;
}

/*
===============
SerializeCordon
===============
*/
void MAPSerializer::SerializeCordon()
{
	if ( !m_cordon )
	{
		return;
	}

	// TODO
}

/*
===============
SerializeBrushFaces
===============
*/
bool MAPSerializer::SerializeBrushFaces( struct qFace_s *faceDef, struct qBrush_s *brushDef )
{
	vec3_t p0, p1, p2;
	vec4_t uAxis, vAxis;

	if ( m_writeMode )
	{
		p0 = faceDef->m_vertices[1].coords;
		p1 = faceDef->m_vertices[0].coords;
		p2 = faceDef->m_vertices[2].coords;

		Sys_SnapVertex( p0.Base() );
		Sys_SnapVertex( p1.Base() );
		Sys_SnapVertex( p2.Base() );

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
		fprintf( m_fileHandle, EXTRA_TAB EXTRA_TAB "( %s %s %s ) ( %s %s %s ) ( %s %s %s )", Sys_PrintMapCoordVector3D( p0 ), Sys_PrintMapCoordVector3D( p1 ), Sys_PrintMapCoordVector3D( p2 ) );
#else
		fprintf( m_fileHandle, EXTRA_TAB EXTRA_TAB "( %g %g %g ) ( %g %g %g ) ( %g %g %g )", Sys_PrintVector3D( p0 ), Sys_PrintVector3D( p1 ), Sys_PrintVector3D( p2 ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

		uAxis = faceDef->m_texDef.m_UAxis;
		vAxis = faceDef->m_texDef.m_VAxis;

		Sys_SnapAxis( 4, uAxis.Base() );
		Sys_SnapAxis( 4, vAxis.Base() );

#if ( MAPVERSION == MAPVERSION_VALVE220 )
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
		fprintf( m_fileHandle, " %s [ %s %s %s %s ] [ %s %s %s %s ] %s %s %s\n",
			faceDef->m_texDef.m_textureName,

			// [
			Sys_PrintAxisVector4D( uAxis ),
			// ]

			// [
			Sys_PrintAxisVector4D( vAxis ),
			// ]

			Sys_PrintValue( faceDef->m_texDef.m_rotate ),
			Sys_PrintValueVector2D( faceDef->m_texDef.m_scale )
		);
#else
		fprintf( m_fileHandle, " %s [ %g %g %g %g ] [ %g %g %g %g ] %g %g %g\n",
			faceDef->m_texDef.m_textureName,

			// [
			Sys_PrintVector4D( uAxis ),
			// ]

			// [
			Sys_PrintVector4D( vAxis ),
			// ]

			faceDef->m_texDef.m_rotate,
			Sys_PrintVector2D( faceDef->m_texDef.m_scale )
		);
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
#elif ( MAPVERSION == MAPVERSION_LEGACY )
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
		fprintf( m_fileHandle, " %s %s %s %s %s %s\n",
			faceDef->m_texDef.m_textureName,

			Sys_PrintAxis( faceDef->m_texDef.m_UAxis.w ),
			Sys_PrintAxis( faceDef->m_texDef.m_VAxis.w ),

			Sys_PrintAxis( faceDef->m_texDef.m_rotate ),
			Sys_PrintValueVector2D( faceDef->m_texDef.m_scale )
		);
#else
		fprintf( m_fileHandle, " %s %g %g %g %g %g\n",
			faceDef->m_texDef.m_textureName,

			faceDef->m_texDef.m_UAxis.w,
			faceDef->m_texDef.m_VAxis.w,

			faceDef->m_texDef.m_rotate,
			Sys_PrintVector2D( faceDef->m_texDef.m_scale )
		);
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
#else
		#error
#endif // ( MAPVERSION == MAPVERSION_VALVE220 )

		return true;
	}
	else
	{
		m_parser->pfnSC_Parse1DMatrix( 3, p0.Base() );
		m_parser->pfnSC_Parse1DMatrix( 3, p1.Base() );
		m_parser->pfnSC_Parse1DMatrix( 3, p2.Base() );

		qPlane_t plane;
		PlaneFromPoints( p0, p1, p2, plane );

		qTexDef_t texDef;
		memset( &texDef, 0, sizeof( qTexDef_t ) );

		m_parser->pfnSC_SetParseFlags( m_parser->pfnSC_GetParseFlags() | PFL_NOERRORS );

		m_parser->pfnSC_GetToken( false );
		strncpy( texDef.m_textureName, m_parser->pfnSC_Token(), sizeof( texDef.m_textureName ) );
		texDef.m_textureName[sizeof( texDef.m_textureName ) - 1] = '\0';

		m_parser->pfnSC_SetParseFlags( m_parser->pfnSC_GetParseFlags() & ~PFL_NOERRORS );

		V_Strupr( texDef.m_textureName );

		if ( m_mapVersion <= MAPVERSION_LEGACY )
		{
			// shiftX shiftY rotation scaleX scaleY

			m_parser->pfnSC_GetToken( false );

			const char *token = m_parser->pfnSC_Token();
			if ( token[0] == '[' && token[1] == '\0' )
			{
				Sys_Warning( "not a version %i file (is it a decompiled BSP output?)", m_mapVersion );

				m_mapVersion = MAPVERSION_VALVE220;
				m_parser->pfnSC_UnGetToken();
			}
			else
			{
				texDef.m_UAxis.w = V_Atof( m_parser->pfnSC_Token() );

				m_parser->pfnSC_GetToken( false );
				texDef.m_VAxis.w = V_Atof( m_parser->pfnSC_Token() );

				m_parser->pfnSC_GetToken( false );
				texDef.m_rotate = -V_Atof( m_parser->pfnSC_Token() );

				m_parser->pfnSC_GetToken( false );
				texDef.m_scale.x = V_Atof( m_parser->pfnSC_Token() );

				m_parser->pfnSC_GetToken( false );
				texDef.m_scale.y = V_Atof( m_parser->pfnSC_Token() );

				texDef.m_textureAlignment = TEXALIGN_QUAKE;
			}
		}

		if ( m_mapVersion >= MAPVERSION_VALVE220 )
		{
			// [ ux uy uz shift ] [ vx vy vz shift ] rotate scaleX scaleY

			// TODO
			m_parser->pfnSC_GetToken( false );

			// [
			m_parser->pfnSC_GetToken( false );
			texDef.m_UAxis.x = V_Atof( m_parser->pfnSC_Token() );
			m_parser->pfnSC_GetToken( false );
			texDef.m_UAxis.y = V_Atof( m_parser->pfnSC_Token() );
			m_parser->pfnSC_GetToken( false );
			texDef.m_UAxis.z = V_Atof( m_parser->pfnSC_Token() );
			m_parser->pfnSC_GetToken( false );
			texDef.m_UAxis.w = V_Atof( m_parser->pfnSC_Token() );
			// ]

			m_parser->pfnSC_MatchToken( "]" );
			if ( m_parser->pfnSC_CheckError() )
			{
				return false;
			}

			m_parser->pfnSC_MatchToken( "[" );
			if ( m_parser->pfnSC_CheckError() )
			{
				return false;
			}

			// [
			m_parser->pfnSC_GetToken( false );
			texDef.m_VAxis.x = V_Atof( m_parser->pfnSC_Token() );
			m_parser->pfnSC_GetToken( false );
			texDef.m_VAxis.y = V_Atof( m_parser->pfnSC_Token() );
			m_parser->pfnSC_GetToken( false );
			texDef.m_VAxis.z = V_Atof( m_parser->pfnSC_Token() );
			m_parser->pfnSC_GetToken( false );
			texDef.m_VAxis.w = V_Atof( m_parser->pfnSC_Token() );
			// ]

			m_parser->pfnSC_MatchToken( "]" );
			if ( m_parser->pfnSC_CheckError() )
			{
				return false;
			}

			m_parser->pfnSC_GetToken( false );
			texDef.m_rotate = V_Atof( m_parser->pfnSC_Token() );

			m_parser->pfnSC_GetToken( false );
			texDef.m_scale.x = V_Atof( m_parser->pfnSC_Token() );

			m_parser->pfnSC_GetToken( false );
			texDef.m_scale.y = V_Atof( m_parser->pfnSC_Token() );

			texDef.m_textureAlignment = TEXALIGN_FACE;
		}

		while ( texDef.m_rotate < 0.0f )
		{
			texDef.m_rotate += 360.0f;
		}

		while ( texDef.m_rotate >= 360.0f )
		{
			texDef.m_rotate -= 360.0f;
		}

		if ( texDef.m_scale.x == 0.0f )
		{
			texDef.m_scale.x = 1.0f;
		}

		if ( texDef.m_scale.y == 0.0f )
		{
			texDef.m_scale.y = 1.0f;
		}

		// Skip all unsupported tokens
		while ( m_parser->pfnSC_TokenAvailable() )
		{
			m_parser->pfnSC_GetToken( false );
		}

		qFace_t *faceDef = Face_Create( m_worldDef, brushDef, &texDef, 0 );
		if ( faceDef )
		{
			faceDef->m_plane = plane;
			return true;
		}
	}

	return false;
}

/*
===============
SerializeBrushes
===============
*/
bool MAPSerializer::SerializeBrushes( struct qBrush_s *brushDef, struct qEntity_s *entityDef )
{
	if ( m_writeMode )
	{
		if ( !brushDef->m_faceList )
		{
			return false;
		}

		if ( FBitSet( m_worldDef->m_editorFlags, 0x100000 ) && FBitSet( brushDef->m_editorFlags, 8 ) )
		{
			return true;
		}

		if ( m_cordon && (
			( brushDef->m_bboxMin.x > m_worldDef->m_vecCordonMax.x - 0.001 ) || ( brushDef->m_bboxMin.y > m_worldDef->m_vecCordonMax.y - 0.001 ) || ( brushDef->m_bboxMin.z > m_worldDef->m_vecCordonMax.z - 0.001 ) ||
			( brushDef->m_bboxMax.x > m_worldDef->m_vecCordonMin.x + 0.001 ) || ( brushDef->m_bboxMax.y > m_worldDef->m_vecCordonMin.y + 0.001 ) || ( brushDef->m_bboxMax.z > m_worldDef->m_vecCordonMin.z + 0.001 ) ) )
		{
			return true;
		}

		fprintf( m_fileHandle, "%s\n", EXTRA_TAB "{" );

		for ( qFace_t *faceDef = brushDef->m_faceList; faceDef != NULL; faceDef = faceDef->next )
		{
			SerializeBrushFaces( faceDef, brushDef );
		}

		fprintf( m_fileHandle, "%s\n", EXTRA_TAB "}" );

		return true;
	}
	else
	{
		m_parser->pfnSC_MatchToken( "{" );

		if ( m_parser->pfnSC_CheckError() )
		{
			return false;
		}

		if ( m_parser->pfnSC_CheckError() )
		{
			return false;
		}

		qBrush_t *newBrushDef = Brush_Create( m_worldDef, entityDef );
		if ( !newBrushDef )
		{
			return false;
		}

		while ( m_parser->pfnSC_GetToken( true ) )
		{
			const char *token = m_parser->pfnSC_Token();

			// Exit if we've reached the final '}'
			if ( token[0] == '}' && token[1] == '\0' )
			{
				break;
			}

			// SC_Parse1DMatrix assumes that we have '(' in the next token
			m_parser->pfnSC_UnGetToken();

			if ( !SerializeBrushFaces( NULL, newBrushDef ) )
			{
				return false;
			}
		};

		return true;
	}

	return false;
}

/*
===============
SerializeEntities
===============
*/
bool MAPSerializer::SerializeEntities( struct qEntity_s *entityDef )
{
	vec3_t origin;
	vec3_t angles;

	if ( m_writeMode )
	{
		// TODO: Flags
		if ( !m_cordon )
		{
#if defined( FINE_OUTPUT )
			fprintf( m_fileHandle, "// entity %i\n", s_entCnt++ );
#endif // FINE_OUTPUT

			fprintf( m_fileHandle, "%s\n", "{" );

			fprintf( m_fileHandle, EXTRA_TAB "\"classname\" \"%s\"\n", entityDef->m_className );

			for ( epair_t *epair = entityDef->epairs; epair != NULL; epair = epair->next )
			{
				if ( !epair->key || !epair->value )
					continue;

				if ( !stricmp( epair->key, entityDef->m_entityKeys[1].key ) )
					continue;

				fprintf( m_fileHandle, EXTRA_TAB "\"%s\" \"%s\"\n", epair->key, epair->value );
			}

			// Brush-based entities must have the brush with ORIGIN texture attached to it
			if ( !entityDef->m_brushList )
			{
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
				fprintf( m_fileHandle, EXTRA_TAB "\"%s\" \"%s %s %s\"\n", entityDef->m_entityKeys[1].key, Sys_PrintMapCoordVector3D( entityDef->m_vecOrigin ) );
#else
				fprintf( m_fileHandle, EXTRA_TAB "\"%s\" \"%g %g %g\"\n", entityDef->m_entityKeys[1].key, Sys_PrintVector3D( entityDef->m_vecOrigin ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
			}

			// Main "worldspawn" entity stores information about the resources
			if ( FBitSet( entityDef->m_editorFlags, EFL_WORLDSPAWN ) )
			{
				fprintf( m_fileHandle, EXTRA_TAB "\"mapversion\" \"%i\"\n", m_mapVersion );
				fprintf( m_fileHandle, EXTRA_TAB "\"wad\" \"%s\"\n", m_packageList );
				fprintf( m_fileHandle, EXTRA_TAB "\"_generator\" \"%s (%s)\"\n", V_VersionString(), "vpHalfLifeAlpha" );
				SerializeCordon();
			}

			for ( qBrush_t *brushDef = entityDef->m_brushList; brushDef != NULL; brushDef = brushDef->next )
			{
				SerializeBrushes( brushDef, entityDef );
			}

			fprintf( m_fileHandle, "%s\n", "}" );

#if defined( FINE_OUTPUT )
			fprintf( m_fileHandle, "\n" );
#endif // FINE_OUTPUT

			return true;
		}

		// TODO: Cordon

		return true;
	}
	else
	{
		int createFlags = ENT_BLDFLG_FULLBUILD;
		char classname[512] = { 0 };

		epair_t *epairList = NULL;

		qEntity_t *newEntityDef = NULL;

		//
		// Find the first block
		//
		m_parser->pfnSC_MatchToken( "{" );
		if ( m_parser->pfnSC_CheckError() )
		{
			return false;
		}

		while ( m_parser->pfnSC_GetToken( true ) )
		{
			const char *token = m_parser->pfnSC_Token();

			// Exit if we've reached the final '}'
			if ( token[0] == '}' && token[1] == '\0' )
				break;

			// Check if entity has embedded brushes
			if ( token[0] == '{' && token[1] == '\0' )
			{
				ClearBits( createFlags, ENT_BLDFLG_FULLBUILD );

				//
				// Entity must exists before brushes are created
				//
				if ( !newEntityDef )
				{
					if ( !classname[0] )
					{
						FreeEpairList( epairList );
						return false;
					}

					if ( !stricmp( classname, "worldspawn" ) )
					{
						SetBits( createFlags, EFL_WORLDSPAWN );
					}

					newEntityDef = Entity_Create( m_worldDef, classname, origin.Base(), createFlags );
					if ( !newEntityDef )
					{
						FreeEpairList( epairList );
						return false;
					}

					newEntityDef->epairs = epairList;
					newEntityDef->m_vecAngles = angles;
				}

				// SerializeBrushes assumes that we have '{' in the next token
				m_parser->pfnSC_UnGetToken();

				if ( !SerializeBrushes( NULL, newEntityDef ) )
				{
					++m_numInvalidSolid;

					m_parser->pfnSC_ResetError();

					// Skip to the next entity
					while ( m_parser->pfnSC_GetToken( true ) )
					{
						if ( m_parser->pfnSC_Token()[0] == '}' )
						{
							break;
						}
					}
				}

				continue;
			}

			if ( newEntityDef )
			{
				m_parser->pfnSC_ParseError( "epairs are not allowed after brushes" );

				m_parser->pfnSC_ResetError();
				m_parser->pfnSC_ResetError();

				// Skip to the next entity
				while ( m_parser->pfnSC_GetToken( true ) )
				{
					if ( m_parser->pfnSC_Token()[0] == '}' )
					{
						break;
					}
				}

				m_parser->pfnSC_UnGetToken();
				return false;
			}

			//
			// Parse key
			//
			char key[64] = { 0 };
			strncpy( key, m_parser->pfnSC_Token(), sizeof( key ) );

			//
			// Parse value
			//
			if ( !m_parser->pfnSC_GetToken( false ) )
			{
				FreeEpairList( epairList );
				return false;
			}
			const char *value = m_parser->pfnSC_Token();

			//
			// Special keys
			//
			if ( !stricmp( key, "classname" ) )
			{
				strncpy( classname, value, sizeof( classname ) );
			}
			else if ( !stricmp( key, "origin" ) )
			{
				(void)sscanf( value, "%f %f %f", &origin.x, &origin.y, &origin.z );
			}
			else if ( !stricmp( key, "mapversion" ) )
			{
				m_mapVersion = V_Atoi( value );
			}
			else if ( !stricmp( key, "wad" ) || !stricmp( key, "_generator" ) )
			{
				// Skip "wad" and "_generator"
			}
			else if ( !stricmp( key, "angles" ) )
			{
				(void)sscanf( value, "%f %f %f", &angles.x, &angles.y, &angles.z );
			}
			else if ( !stricmp( key, "angle" ) )
			{
				float angle = V_Atof( value );

				if ( angle == -1 ) // Up
				{
					angles[PITCH] = -90;
					angles[YAW] = 0;
					angles[ROLL] = 0;
				}
				else if ( angle == -2 ) // Down
				{
					angles[PITCH] = 90;
					angles[YAW] = 0;
					angles[ROLL] = 0;
				}
				else
				{
					angles[PITCH] = 0;
					angles[YAW] = angle;
					angles[ROLL] = 0;
				}
			}
			else
			{
				AddEpairToList( epairList, key, value );
			}
		}

		if ( !newEntityDef )
		{
			if ( !classname[0] )
			{
				FreeEpairList( epairList );
				return false;
			}

			if ( !stricmp( classname, "worldspawn" ) )
			{
				SetBits( createFlags, EFL_WORLDSPAWN );
			}

			newEntityDef = Entity_Create( m_worldDef, classname, origin.Base(), createFlags );
			if ( !newEntityDef )
			{
				FreeEpairList( epairList );
				return false;
			}

			newEntityDef->epairs = epairList;
			newEntityDef->m_vecAngles = angles;
		}

		Entity_Build( newEntityDef, m_mapVersion >= MAPVERSION_VALVE220 ? ENT_BLDFLG_FULLBUILD | ENT_BLDFLG_BRUSH_FACESNADOVERLAYS : ENT_BLDFLG_FULLBUILD | ENT_BLDFLG_BRUSH_FACESNADOVERLAYS | ENT_BLDFLG_BIT3 );

		return true;
	}

	return false;
}

/*
===============
SerializePathNodes
===============
*/
bool MAPSerializer::SerializePathNodes( struct qPath_s *pathDef )
{
	if ( !m_writeMode )
	{
		return true;
	}

	if ( !pathDef->m_nodeList )
		return true;

	if ( !pathDef->m_pathClassname || !pathDef->m_pathClassname[0] )
		return true;

	if ( m_cordon )
	{
		const vec3_t &bbmin = pathDef->m_bboxMin;
		const vec3_t &bbmax = pathDef->m_bboxMax;

		if (( bbmin.x > m_worldDef->m_vecCordonMax.x - 0.001 ) || ( bbmin.y > m_worldDef->m_vecCordonMax.y - 0.001 ) || ( bbmin.z > m_worldDef->m_vecCordonMax.z - 0.001 ) ||
			( bbmax.x < m_worldDef->m_vecCordonMin.x + 0.001 ) || ( bbmax.y < m_worldDef->m_vecCordonMin.y + 0.001 ) || ( bbmax.z < m_worldDef->m_vecCordonMin.z + 0.001 ))
		{
			return true;
		}

		for ( qNode_s *nodeDef = pathDef->m_nodeList; nodeDef != NULL; nodeDef = nodeDef->next )
		{
			const vec3_t &org = nodeDef->m_vecOrigin;

			if (( org.x >= m_worldDef->m_vecCordonMin.x + 0.001f && org.y >= m_worldDef->m_vecCordonMin.y + 0.001f && org.z >= m_worldDef->m_vecCordonMin.z + 0.001f ) &&
				( org.x <= m_worldDef->m_vecCordonMax.x - 0.001f && org.y <= m_worldDef->m_vecCordonMax.y - 0.001f && org.z <= m_worldDef->m_vecCordonMax.z - 0.001f ))
			{
				return true;
			}
		}
	}

	int nodeIdx = 0;
	for ( qNode_t *nodeDef = pathDef->m_nodeList; nodeDef != NULL; nodeDef = nodeDef->next )
	{
		fprintf( m_fileHandle, "%s\n", "{" );

		fprintf( m_fileHandle, "\"classname\" \"%s\"\n", pathDef->m_pathClassname );

		if ( nodeDef->m_nameOverride && nodeDef->m_nameOverride[0] )
		{
			fprintf( m_fileHandle, "\"targetname\" \"%s\"\n", nodeDef->m_nameOverride );

			if ( nodeDef == pathDef->m_nodeList )
			{
				++nodeIdx;
			}
		}
		else
		{
			if ( nodeIdx != 0 )
			{
				fprintf( m_fileHandle, "\"targetname\" \"%s_%i\"\n", pathDef->m_pathName, nodeIdx );
			}
			else
			{
				fprintf( m_fileHandle, "\"targetname\" \"%s\"\n", pathDef->m_pathName );
			}

			++nodeIdx;
		}

		if ( nodeDef->next )
		{
			if ( nodeDef->next->m_nameOverride && nodeDef->next->m_nameOverride[0] )
			{
				fprintf( m_fileHandle, "\"target\" \"%s\"\n", nodeDef->next->m_nameOverride );
			}
			else
			{
				fprintf( m_fileHandle, "\"target\" \"%s_%i\"\n", pathDef->m_pathName, nodeIdx );
			}
		}
		else if ( pathDef->m_pathDirection == 1 )
		{
			const char *target = pathDef->m_nodeList->m_nameOverride;

			if ( !target || !target[0] )
			{
				target = pathDef->m_pathName;
			}

			fprintf( m_fileHandle, "\"target\" \"%s\"\n", target );
		}
		else if ( pathDef->m_pathDirection == 2 && nodeDef->prev )
		{
			fprintf( m_fileHandle, "\"target\" \"%s_%i\"\n", pathDef->m_pathName, nodeIdx );
		}

		for ( epair_s *epairs = nodeDef->epairs; epairs != NULL; epairs = epairs->next )
		{
			if ( epairs->key && epairs->value )
			{
				fprintf( m_fileHandle, "\"%s\" \"%s\"\n", epairs->key, epairs->value );
			}
		}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
		fprintf( m_fileHandle, "\"origin\" \"%s %s %s\"\n", Sys_PrintMapCoordVector3D( nodeDef->m_vecOrigin ) );
#else
		fprintf( m_fileHandle, "\"origin\" \"%g %g %g\"\n", Sys_PrintVector3D( nodeDef->m_vecOrigin ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

		const vec3_t &ang = nodeDef->m_vecAngles;
		if ( ang.x != 0.f || ang.y != 0.f || ang.z != 0.f )
		{
			fprintf( m_fileHandle, "\"angles\" \"%.0f %.0f %.0f\"\n", Sys_PrintVector3D( ang ) );
		}

		if ( nodeDef->m_fireEntityOnPass && nodeDef->m_fireEntityOnPass[0] )
		{
			fprintf( m_fileHandle, "\"message\" \"%s\"\n", nodeDef->m_fireEntityOnPass );
		}

		fprintf( m_fileHandle, "%s\n", "}" );
	}

	if ( pathDef->m_pathDirection == 2 )
	{
		for ( qNode_s *nodeDef = pathDef->m_lastNode->prev; nodeDef != NULL; nodeDef = nodeDef->prev )
		{
			fprintf( m_fileHandle, "%s\n", "{" );

			fprintf( m_fileHandle, "\"classname\" \"%s\"\n", pathDef->m_pathClassname );

			fprintf( m_fileHandle, "\"targetname\" \"%s_%i\"\n", pathDef->m_pathName, nodeIdx++ );

			if ( nodeDef->prev->prev != NULL )
			{
				fprintf( m_fileHandle, "\"target\" \"%s_%i\"\n", pathDef->m_pathName, nodeIdx );
			}
			else
			{
				const char *target = pathDef->m_nodeList->m_nameOverride;

				if ( !target || !target[0] )
				{
					target = pathDef->m_pathName;
				}

				fprintf( m_fileHandle, "\"target\" \"%s\"\n", target );
			}

			for ( epair_s *epairs = nodeDef->epairs; epairs != NULL; epairs = epairs->next )
			{
				if ( epairs->key && epairs->value )
				{
					fprintf( m_fileHandle, "\"%s\" \"%s\"\n", epairs->key, epairs->value );
				}
			}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
			fprintf( m_fileHandle, "\"origin\" \"%s %s %s\"\n", Sys_PrintMapCoordVector3D( nodeDef->m_vecOrigin ) );
#else
			fprintf( m_fileHandle, "\"origin\" \"%g %g %g\"\n", Sys_PrintVector3D( nodeDef->m_vecOrigin ) );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

			const vec3_t &ang = nodeDef->m_vecAngles;
			if ( ang.x != 0.f || ang.y != 0.f || ang.z != 0.f )
			{
				fprintf( m_fileHandle, "\"angles\" \"%.0f %.0f %.0f\"\n", Sys_PrintVector3D( ang ) );
			}

			if ( nodeDef->m_fireEntityOnPass && nodeDef->m_fireEntityOnPass[0] )
			{
				fprintf( m_fileHandle, "\"message\" \"%s\"\n", nodeDef->m_fireEntityOnPass );
			}

			fprintf( m_fileHandle, "%s\n", "}" );
		}
	}

	return true;
}
