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
CMapWorld::CMapWorld()
{
	m_gameConfig = NULL; //gameConfig;

	m_vecCordonMin = m_vecCordonMax = m_skyData.m_skyAxis = vec3_origin;

	m_project = NULL; //project;

	m_octree = NULL;

	/*
  v4 = operator new[](a1: 0x80000u);
  v5 = (_QWORD *)v4;
  do
    *v5++ = 0;
  while ( v5 != (_QWORD *)(v4 + 0x80000) );
  CMapWorld_gap = v4;

  &CMapWorld_gap[8] = 1;
  &CMapWorld_gap[12] = 1;
  &CMapWorld_gap[16] = 0;
  &CMapWorld_gap[20] = 0;
	*/

	m_lastUsedEntityEditorId = 0;
	m_lastUsedBrushEditorId = 0;
	m_lastUsedFaceEditorId = 0;
	m_lastUsedNodeEditorId = 0;
	m_lastUsedPathEditorId = 0;
	m_lastUsedCameraEditorId = 0;
	//CMapWorld_gap3 = 0;
	m_lastUsedVisGroupEditorId = 0;
	//CMapWorld_gap4 = 0;

	//&CMapWorld_gap4[8] = 0;
	//&CMapWorld_gap4[16] = 0;
	//&CMapWorld_gap4[24] = 0;
	//&CMapWorld_gap4[32] = 0;
	//&CMapWorld_gap4[40] = 0;

	//m_visGroupList = NULL;

	memset( this, 0, sizeof( qWorld_s ) );
	m_mapWorld = this;

	//updateSelectionMode();
}

/*
===============
Destructor
===============
*/
CMapWorld::~CMapWorld()
{
	clear();

	if ( m_octree )
	{
		delete m_octree;
	}

	/*
  v3 = *(void **)this->CMapWorld_gap;
  if ( v3 != nullptr )
    operator delete[](a1: v3);
	*/
}

/*
===============
createVisGroup
===============
*/
CVisGroup *CMapWorld::createVisGroup( const char *visGroupName )
{
	CVisGroup *visGroup = new CVisGroup;
	visGroup->setName( visGroupName );

	m_visGroupList.push_back( visGroup );

	//setModified( true );

	return visGroup;
}

/*
===============
pushGroup
===============
*/
void CMapWorld::pushGroup( qGroup_s *groupDef )
{
	//groupDef->m_mapGroup = alloc_selectionId();

	groupDef->prev = NULL;
	groupDef->next = m_groupList;
	if ( m_groupList != NULL )
	{
		m_groupList->prev = groupDef;
	}
	m_groupList = groupDef;
}

/*
===============
buildPackageList
===============
*/
bool CMapWorld::buildPackageList( char **outBuf, bool listSeparator, int removeVolumePrefix )
{
	if ( !outBuf )
	{
		return false;
	}

	Sys_Free( *outBuf );

	// TODO
	*outBuf = Sys_AllocString( "" );

	return true;
}

/*
===============
clear
===============
*/
void CMapWorld::clear()
{
	SetBits( m_editorFlags, EFL_FRESHWORLD );

	removeAllEntities();
	removeAllVisGroups();
	removeAllCameras();
	removeAllPaths();
	//unloadPointFile();
}

/*
===============
removeAllEntities
===============
*/
void CMapWorld::removeAllEntities()
{
	qEntity_s *nextEntityDef = NULL;
	for ( qEntity_s *entityDef = m_entityList; entityDef != NULL; entityDef = nextEntityDef )
	{
		nextEntityDef = entityDef->next;

		for ( qBrush_s *brushDef = entityDef->m_brushList; brushDef != NULL; brushDef = brushDef->next )
		{
			brushDef->m_mapBrush->removeFromAllVisGroups();
		}

		entityDef->m_mapEntity->removeFromAllVisGroups();

		if ( entityDef->m_mapEntity )
		{
			delete entityDef->m_mapEntity;
		}
	}

	qGroup_s *nextGroupDef = NULL;
	for ( qGroup_s *groupDef = m_groupList; groupDef != NULL; groupDef = nextGroupDef )
	{
		nextGroupDef = groupDef->next;

		if ( groupDef->m_mapGroup )
		{
			delete groupDef->m_mapGroup;
		}
	}

	m_entityList = NULL;
	m_groupList = NULL;
	m_lastEntity = NULL;
	m_selEntityList = NULL;
	m_selBrushList = NULL;
	m_selGroupList = NULL;
	m_selFaceList = NULL;
}

/*
===============
removeAllVisGroups
===============
*/
void CMapWorld::removeAllVisGroups()
{
	for ( auto visGroup : m_visGroupList )
	{
		if ( visGroup )
		{
			delete visGroup;
		}
	}

	m_lastUsedVisGroupEditorId = 0;
}

/*
===============
removeAllCameras
===============
*/
void CMapWorld::removeAllCameras()
{
	qCamera_s *nextCameraDef = NULL;
	for ( qCamera_s *cameraDef = m_cameraList; cameraDef != NULL; cameraDef = nextCameraDef )
	{
		nextCameraDef = cameraDef->next;

		if ( cameraDef->m_mapCamera )
		{
			delete cameraDef->m_mapCamera;
		}
	}

	m_cameraList = NULL;
	m_selCamera = NULL;
}

/*
===============
removeAllPaths
===============
*/
void CMapWorld::removeAllPaths()
{
	qPath_s *nextPathDef = NULL;
	for ( qPath_s *pathDef = m_pathList; pathDef != NULL; pathDef = nextPathDef )
	{
		nextPathDef = pathDef->next;

		if ( pathDef->m_mapPath )
		{
			delete pathDef->m_mapPath;
		}
	}

	m_pathList = NULL;
	m_selNodeList = NULL;
}


/*
===============
Constructor
===============
*/
CVisGroup::CVisGroup()
{
	m_name = NULL;
	m_cbColor = rgba_t( 0, 0, 0, 0 );
	m_editorId = 0;
	m_visible = false;

	gap = 0;
}

/*
===============
Destructor
===============
*/
CVisGroup::~CVisGroup()
{
	Sys_Free( m_name );
}

/*
===============
setName
===============
*/
void CVisGroup::setName( const char *name )
{
	if ( m_name && stricmp( m_name, name ) != 0 )
	{
		free( m_name );
		m_name = NULL;
	}

	if ( !m_name )
	{
		m_name = Sys_AllocString( name );
	}
}


/*
===============
Constructor
===============
*/
CBackgroundImage::CBackgroundImage()
{
	m_filePath = NULL;
}

/*
===============
Destructor
===============
*/
CBackgroundImage::~CBackgroundImage()
{
	Sys_Free( m_filePath );
}
