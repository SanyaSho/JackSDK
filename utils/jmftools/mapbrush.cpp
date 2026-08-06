//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <algorithm>

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
CMapBrush::CMapBrush()
{
	m_brushData = (qBrush_s *)Sys_Malloc( sizeof( qBrush_s ) );

	m_selectionId = -1;

	m_cbColor = m_cbFaceColor = rgba_t( 0, 0, 0, 255 );

	m_brushSize = vec3_origin;

	//m_visGroupList = NULL;

	memset( m_brushData, 0, sizeof( *m_brushData ) );
	m_brushData->m_mapBrush = this;
}

/*
===============
Destructor
===============
*/
CMapBrush::~CMapBrush()
{
	//removeDecals();

	removeFromAllVisGroups();

	if ( m_brushData->m_patch )
	{
		if ( m_brushData->m_patch->m_mapPatch )
		{
			delete m_brushData->m_patch->m_mapPatch;
		}
	}

	if ( m_brushData->m_overlay )
	{
		if ( m_brushData->m_overlay->m_mapOverlay )
		{
			delete m_brushData->m_overlay->m_mapOverlay;
		}
	}

	qFace_s *nextFaceDef = NULL;
	for ( qFace_s *faceDef = m_brushData->m_faceList; faceDef != NULL; faceDef = nextFaceDef )
	{
		nextFaceDef = faceDef->next;

		if ( faceDef->m_mapFace )
		{
			delete faceDef->m_mapFace;
		}
	}

	Sys_Free( m_brushData );
}

/*
===============
addToVisGroup
===============
*/
void CMapBrush::addToVisGroup( CVisGroup *visGroup )
{
	if ( std::find( m_visGroupList.begin(), m_visGroupList.end(), visGroup ) == m_visGroupList.end() )
	{
		m_visGroupList.push_back( visGroup );
	}
}

/*
===============
removeFromAllVisGroups
===============
*/
void CMapBrush::removeFromAllVisGroups()
{
}


/*
===============
Constructor
===============
*/
CMapOverlay::CMapOverlay()
{
	m_overlayData = (qOverlay_s *)Sys_Malloc( sizeof( qOverlay_s ) );

	m_unkFlt1 = 1.f;
	m_unknownVec1 = vec3_origin;

	memset( m_overlayData, 0, sizeof( qOverlay_s ) );
	m_overlayData->m_mapOverlay = this;
}

/*
===============
Destructor
===============
*/
CMapOverlay::~CMapOverlay()
{
	if ( m_overlayData->m_shaderInfo )
	{
		--*m_overlayData->m_shaderInfo->gap;
	}

	Sys_Free( m_overlayData );
}

/*
===============
setTexDef
===============
*/
void CMapOverlay::setTexDef( const qTexDef_s &texDef )
{
	m_overlayData->m_texDef = texDef;
}


/*
===============
Constructor
===============
*/
CMapPatch::CMapPatch( int numColumns, int numRows )
{
	m_patchData = (qPatch_s *)Sys_Malloc( sizeof( qPatch_s ) );

	if ( ( numColumns & 1 ) == 0 )
	{
		numColumns = std::clamp( numColumns + 1, 3, 31 );
	}

	if ( ( numRows & 1 ) == 0 )
	{
		numRows = std::clamp( numRows + 1, 3, 31 );
	}

	memset( m_patchData, 0, sizeof( *m_patchData ) );
	m_patchData->m_mapPatch = this;

	m_patchData->m_numColumns = numColumns;
	m_patchData->m_numRows = numRows;
}

/*
===============
Destructor
===============
*/
CMapPatch::~CMapPatch()
{
	if ( m_patchData->m_shaderInfo )
	{
		--*m_patchData->m_shaderInfo->gap;
	}

	//freePatchPoints();

	Sys_Free( m_patchData );
}

/*
===============
setTexDef
===============
*/
void CMapPatch::setTexDef( const qTexDef_s &texDef )
{
	m_patchData->m_texDef = texDef;
}
