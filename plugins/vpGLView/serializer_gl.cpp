//=====================================================================================//
//
// Purpose: GLView plugin for J.A.C.K.
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

// Plugin API
#include "PluginMeta.h"

#include "DataSerializer.h"
#include "serializer_gl.h"

#include "vstdlib_static.h"

static vec3_t vec3_origin( 0, 0, 0 );

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
/*
===============
ExportGL
===============
*/
bool ExportGL( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
{
	GLSerializer glSerializer( filePath, seekOffset, readLimit, worldDef );
	return glSerializer.Export();
}

/*
===============
ImportGL
===============
*/
bool ImportGL( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
{
	GLSerializer glSerializer( filePath, seekOffset, readLimit, worldDef );
	return glSerializer.Import();
}
#else
/*
===============
ExportGL
===============
*/
bool ExportGL( const char *filePath, struct qWorld_s *worldDef )
{
	GLSerializer glSerializer( filePath, worldDef );
	return glSerializer.Export();
}

/*
===============
ImportGL
===============
*/
bool ImportGL( const char *filePath, struct qWorld_s *worldDef )
{
	GLSerializer glSerializer( filePath, worldDef );
	return glSerializer.Import();
}
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

/*
===============
GLSerializer
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
GLSerializer::GLSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
	: Serializer( filePath, seekOffset, readLimit, worldDef, FMODE_PARSERAPI )
#else
GLSerializer::GLSerializer( const char *filePath, struct qWorld_s *worldDef )
	: Serializer( filePath, worldDef, FMODE_PARSERAPI )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
}

/*
===============
~GLSerializer
===============
*/
GLSerializer::~GLSerializer()
{
	if ( m_fileHandle )
	{
		if ( m_writeMode )
		{
			fflush( m_fileHandle );
		}

		fclose( m_fileHandle );
		m_fileHandle = NULL;
	}
}

/*
===============
Export
===============
*/
bool GLSerializer::Export()
{
	return false;
}

/*
===============
Import
===============
*/
bool GLSerializer::Import()
{
	if ( !FBitSet( m_worldDef->m_editorFlags, ( 1 << 23 ) ) )
	{
		Sys_Printf( "Loading: \"%s\"", m_filePath );
	}

	if ( !OpenForRead() )
	{
		return false;
	}

	qEntity_t *worldSpawn = Entity_Create( m_worldDef, "worldspawn", vec3_origin.Base(), EFL_WORLDSPAWN );
	if ( !worldSpawn )
	{
		return false;
	}

	while ( m_parser->pfnSC_GetToken( true ) )
	{
		m_parser->pfnSC_UnGetToken();

		SerializeGLViewFile( worldSpawn );
	}

	m_parser->pfnSC_EndOfParsing();

	Entity_Build( worldSpawn, ENT_BLDFLG_FULLBUILD | ENT_BLDFLG_BRUSH_FACESNADOVERLAYS );

	return true;
}

/*
===============
SerializeGLViewFile
===============
*/
void GLSerializer::SerializeGLViewFile( qEntity_s *worldSpawn )
{
	m_parser->pfnSC_GetToken( true );
	int numVerts = V_Atoi( m_parser->pfnSC_Token() );

	if ( numVerts <= 0 || numVerts > 4 )
	{
		m_parser->pfnSC_ParseError( "Invalid vertex count %s", m_parser->pfnSC_Token() );
	}

	if ( numVerts < 4 )
	{
		Sys_Warning( "Data declared at line %d has less than 4 verts on it, output might be wrong!", m_parser->pfnSC_Line() );
	}

	qBrush_t *brushDef = Brush_Create( m_worldDef, worldSpawn );
	if ( !brushDef )
	{
		return;
	}

	vec3_t *pointList = (vec3_t *)Sys_Malloc( sizeof( vec3_t ) * numVerts );
	if ( !pointList )
	{
		return;
	}

	for ( int i = 0; i < numVerts; i++ )
	{
		// Coords
		if ( !m_parser->pfnSC_GetToken( true ) )
			break;
		pointList[i].x = V_Atof( m_parser->pfnSC_Token() );

		if ( !m_parser->pfnSC_GetToken( true ) )
			break;
		pointList[i].y = V_Atof( m_parser->pfnSC_Token() );

		if ( !m_parser->pfnSC_GetToken( true ) )
			break;
		pointList[i].z = V_Atof( m_parser->pfnSC_Token() );

		// Color
		if ( !m_parser->pfnSC_GetToken( true ) )
			break;
		if ( !m_parser->pfnSC_GetToken( true ) )
			break;
		if ( !m_parser->pfnSC_GetToken( true ) )
			break;
	}

	qTexDef_t texDef;
	memset( &texDef, 0, sizeof( texDef ) );

	texDef.m_scale = vec2_t( 1.f, 1.f );
	texDef.m_textureAlignment = TEXALIGN_QUAKE;

	strncpy( texDef.m_textureName, "NULL", sizeof( texDef.m_textureName ) );
	texDef.m_textureName[sizeof( texDef.m_textureName ) - 1] = '\0';

	qOverlay_t *overlayDef = Overlay_Create( brushDef, &texDef );
	if ( overlayDef )
	{
		for ( int i = 0; i < numVerts; i++ )
		{
			overlayDef->m_data[i].coords = pointList[i];
		}

		// Try to fixup the wrong geometry
		if ( numVerts == 3 )
		{
			overlayDef->m_data[3].coords = pointList[2];
		}
	}

	Sys_Free( pointList );
}
