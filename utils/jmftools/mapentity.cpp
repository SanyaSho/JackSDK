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
CMapEntity::CMapEntity()
{
	m_entityData = (qEntity_s *)Sys_Malloc( sizeof( qEntity_s ) );

	m_currentGDClass = NULL;

	m_selectionId = -1;

	m_cbColor = rgba_t( 0, 0, 0, 255 );

	m_vecData1 = NULL;
	//m_visGroupList = NULL;

	memset( m_entityData, 0, sizeof( *m_entityData ) );
	m_entityData->m_mapEntity = this;

	reset();
	clearEpairs();
}

/*
===============
Destructor
===============
*/
CMapEntity::~CMapEntity()
{
	removeFromAllVisGroups();

	qBrush_s *nextBrushDef = NULL;
	for ( qBrush_s *brushDef = m_entityData->m_brushList; brushDef != NULL; brushDef = nextBrushDef )
	{
		nextBrushDef = brushDef->next;

		if ( brushDef->m_mapBrush )
		{
			delete brushDef->m_mapBrush;
		}
	}

	freeUserData();

	clearEpairs();

	Sys_Free( m_entityData->m_className );
	m_entityData->m_className = NULL;

	Sys_Free( m_entityData->m_targetName );
	m_entityData->m_targetName = NULL;

	//CEntityKeys::clear( m_entityData->m_entityKeys );

	//Spline_Free( m_entityData );

	Sys_Free( m_entityData );
	m_entityData = NULL;
}

/*
===============
removeFromAllVisGroups
===============
*/
void CMapEntity::removeFromAllVisGroups()
{
}

/*
===============
reset
===============
*/
void CMapEntity::reset()
{
	if ( !m_entityData->m_brushList )
	{
		m_entityData->m_bboxMin = m_entityData->m_vecOrigin - 8.f;
		m_entityData->m_bboxMax = m_entityData->m_vecOrigin + 8.f;

		if ( FBitSet( m_entityData->m_editorFlags, EFL_WORLDSPAWN ) )
		{
			m_cbColor = rgba_t( 255, 255, 255, 255 );
		}
		else
		{
			m_cbColor = rgba_t( 220, 30, 220, 255 );
		}
	}
}

/*
===============
updateKeys
===============
*/
void CMapEntity::updateKeys( bool keepkeys )
{
}

/*
===============
freeUserData
===============
*/
void CMapEntity::freeUserData()
{
	if ( !m_entityData->m_drawData )
	{
		m_entityData->m_modelType = mod_brush;
		return;
	}

	switch ( m_entityData->m_modelType )
	{
	case mod_brush:
		break;
	case mod_sprite:
	{
		qSpriteDrawData_s *drawData = (qSpriteDrawData_s *)m_entityData->m_drawData;

		if ( drawData->m_spriteData )
		{
			--drawData->m_spriteData->m_refCount;

			if ( drawData->m_spriteData->m_spriteShader )
			{
				--*drawData->m_spriteData->m_spriteShader->gap;
			}
		}
		break;
	}
	case mod_decal:
	{
		qDecal_s *drawData = (qDecal_s *)m_entityData->m_drawData;

		--drawData->m_decalShader->m_refCount;
		//Decal_FreeFragments( drawData );
		break;
	}
	case mod_studio:
	{
		qStudioDrawData_s *drawData = (qStudioDrawData_s *)m_entityData->m_drawData;

		if ( drawData->m_studioData1 )
		{
			--drawData->m_studioData1->m_refCount;
		}

		if ( drawData->m_studioData2 )
		{
			--drawData->m_studioData2->m_refCount;
		}
		break;
	}
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	case mod_particles:
	{
		qParticlesDrawData_s *drawData = (qParticlesDrawData_s *)m_entityData->m_drawData;

		if ( drawData->m_particlesData )
		{
			--drawData->m_particlesData->m_refCount;
		}
		break;
	}
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
	};

	free( m_entityData->m_drawData );
	m_entityData->m_drawData = NULL;

	m_entityData->m_modelType = mod_brush;
}
