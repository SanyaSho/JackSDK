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
CMapGroup::CMapGroup()
{
	m_groupData = (qGroup_s *)Sys_Malloc( sizeof( qGroup_s ) );

	m_selectionId = -1;

	m_cbColor1 = m_cbColor2 = rgba_t( 0, 0, 0, 255 );

	memset( m_groupData, 0, sizeof( *m_groupData ) );
	m_groupData->m_mapGroup = this;

	// TODO Random RGB for m_cbColor1
}

/*
===============
Destructor
===============
*/
CMapGroup::~CMapGroup()
{
	Sys_Free( m_groupData );
}
