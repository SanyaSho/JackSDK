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
CMapPath::CMapPath()
{
	m_pathData = (qPath_t *)Sys_Malloc( sizeof( qPath_t ) );

	m_cbColor = rgba_t( 0, 0, 0, 255 );

	m_unknown = false;

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
