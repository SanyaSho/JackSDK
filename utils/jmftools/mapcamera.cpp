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
CMapCamera::CMapCamera()
{
	m_cameraData = (qCamera_s *)Sys_Malloc( sizeof( qCamera_s ) );

	m_selectionId = -1;

	m_cbColor1 = m_cbColor2 = rgba_t( 0, 0, 0, 255 );

	m_vecAngles = vec3_origin;

	m_length = 0.f;

	memset( m_cameraData, 0, sizeof( *m_cameraData ) );
	m_cameraData->m_mapCamera = this;

	// TODO Random RGB for m_cbColor
}

/*
===============
Destructor
===============
*/
CMapCamera::~CMapCamera()
{
}

/*
===============
setup
===============
*/
void CMapCamera::setup( const vec3_t &origin, const vec3_t &angles )
{
	m_cameraData->m_vecOrigin = origin;
	m_cameraData->m_vecAngles = angles;

	m_vecAngles = angles;

	float dist = sqrt(
		( angles[0] - origin[0] * angles[0] - origin[0] )
		+
		( angles[1] - origin[1] * angles[1] - origin[1] )
		+
		( angles[2] - origin[2] * angles[2] - origin[2] )
	);
	if ( dist >= FLT_EPSILON )
	{
		m_length = dist;
	}
	else
	{
		m_length = 1.f;
	}
}
