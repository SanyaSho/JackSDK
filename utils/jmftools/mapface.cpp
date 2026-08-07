//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>

// Plugin API
#define PLUGINEDITORFUNCTIONS_H
#define ENTITYAPI_NO_INLINEFUNCS
#include "PluginMeta.h"

#include "logger.h"
#include "math.h"
#include "mem.h"

// Containers
#include "mapbrush.h"
#include "mapcamera.h"
#include "mapentity.h"
#include "mapface.h"
#include "mapgroup.h"
#include "mappath.h"
#include "mapworld.h"

/*
===============
Constructor
===============
*/
CMapFace::CMapFace( int vertexCount )
{
	m_faceData = (qFace_s *)Sys_Malloc( sizeof( qFace_s ) );

	m_selectionId = -1;

	m_cbColor = rgba_t( 0, 0, 0, 255 );

	m_unknownFloat = 1.f;

	memset( m_faceData, 0, sizeof( *m_faceData ) );

	if ( vertexCount > 0 )
	{
		m_faceData->m_vertices = (qVertex_s *)Sys_Malloc( vertexCount * sizeof( qVertex_s ) );
	}

	m_faceData->m_mapFace = this;

	m_faceData->m_vertexCount = vertexCount;

	if ( vertexCount == 3 )
	{
		m_unknownInt = 4;
	}
	else
	{
		m_unknownInt = 2 * ( vertexCount != 4 ) + 7;
	}
}

/*
===============
Destructor
===============
*/
CMapFace::~CMapFace()
{
	if ( m_faceData->m_shaderInfo )
	{
		--*m_faceData->m_shaderInfo->gap;
	}

	Sys_Free( m_faceData->m_vertices );
	Sys_Free( m_faceData );
}

/*
===============
setTexDef
===============
*/
void CMapFace::setTexDef( const qTexDef_s &texDef )
{
	m_faceData->m_texDef = texDef;
}
