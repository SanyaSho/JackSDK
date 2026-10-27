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

#include "vstdlib_static.h"

/*
===============
Constructor
===============
*/
CMapPath::CMapPath()
{
	m_pathData = (qPath_t *)Sys_Malloc( sizeof( qPath_t ) );

	m_cbColor = rgba_t( 0, 0, 0, 255 );

	m_spline = false;

	memset( m_pathData, 0, sizeof( *m_pathData ) );
	m_pathData->m_mapPath = this;
}

/*
===============
Destructor
===============
*/
CMapPath::~CMapPath()
{
	Sys_Free( m_pathData->m_pathClassname );
	Sys_Free( m_pathData->m_pathName );

	qNode_s *nextNodeDef = NULL;
	for ( qNode_s *nodeDef = m_pathData->m_nodeList; nodeDef != NULL; nodeDef = nextNodeDef )
	{
		nextNodeDef = nodeDef->next;

		if ( nodeDef->m_mapNode )
		{
			delete nodeDef->m_mapNode;
		}
	}

	Sys_Free( m_pathData );
}

/*
===============
build
===============
*/
void CMapPath::build( int buildFlags )
{
	m_cbColor = rgba_t( 220, 30, 220, 255 );

	if ( m_pathData->m_ownerWorld )
	{
		// ...
	}

	V_ClearBounds( m_pathData->m_bboxMin.Base(), m_pathData->m_bboxMax.Base() );

	for ( qNode_s *nodeDef = m_pathData->m_nodeList; nodeDef != NULL; nodeDef = nodeDef->next )
	{
		nodeDef->m_mapNode->m_cbColor = m_cbColor;

		nodeDef->m_mapNode->updateParms();

		vec3_t boxMin, boxMax;

		boxMax = nodeDef->m_vecOrigin + 8.f;
		boxMin = nodeDef->m_vecOrigin - 8.f;

		V_AddBoxToBounds( boxMin.Base(), boxMax.Base(), m_pathData->m_bboxMin.Base(), m_pathData->m_bboxMax.Base() );
	}

	ClearBits( m_pathData->m_editorFlags, EFL_DIRTY );
}


/*
===============
Constructor
===============
*/
CMapNode::CMapNode()
{
	m_nodeData = (qNode_s *)Sys_Malloc( sizeof( qNode_s ) );

	m_selectionId = -1;

	m_cbColor = rgba_t( 0, 0, 0, 255 );

	m_nodeData = (qNode_s *)Sys_Malloc( sizeof( qNode_s ) );

	memset( m_nodeData, 0, sizeof( *m_nodeData ) );
	m_nodeData->m_mapNode = this;
}

/*
===============
Destructor
===============
*/
CMapNode::~CMapNode()
{
	clearEpairs();

	Sys_Free( m_nodeData );
}

/*
===============
updateParms
===============
*/
void CMapNode::updateParms()
{
	epair_s *epair = NULL;

	m_nodeData->m_speed = 0.f;
	epair = findEpairByKey( "speed" );
	if ( epair )
	{
		m_nodeData->m_speed = strtod( epair->value, NULL );
	}

	m_nodeData->m_yaw_speed = 0.f;
	epair = findEpairByKey( "yaw_speed" );
	if ( epair )
	{
		m_nodeData->m_yaw_speed = strtod( epair->value, NULL );
	}

	m_nodeData->m_wait = 0.f;
	epair = findEpairByKey( "wait" );
	if ( epair )
	{
		m_nodeData->m_wait = strtod( epair->value, NULL );
	}

	m_nodeData->m_fov = 0.f;
	epair = findEpairByKey( "fov" );
	if ( epair )
	{
		m_nodeData->m_fov = strtod( epair->value, NULL );
	}

	m_nodeData->m_spawnflags = 0.f;
	epair = findEpairByKey( "spawnflags" );
	if ( epair )
	{
		m_nodeData->m_spawnflags = strtol( epair->value, NULL, 10 );
	}
}

/*
===============
findEpairByKey
===============
*/
epair_s *CMapNode::findEpairByKey( const char *key )
{
	for ( epair_s *epair = m_nodeData->epairs; epair != NULL; epair = epair->next )
	{
		if ( epair->key != NULL && !stricmp( epair->key, key ) )
		{
			return epair;
		}
	}

	return NULL;
}
