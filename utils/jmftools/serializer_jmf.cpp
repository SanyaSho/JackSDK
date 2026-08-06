//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#include <stdio.h>
#include <assert.h>
#include <memory>

// Plugin API
#define PLUGINEDITORFUNCTIONS_H
#define ENTITYAPI_NO_INLINEFUNCS
#include "PluginMeta.h"

#include "DataSerializer.h"
#include "serializer_jmf.h"

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

/* This struct is written as a 124 byte blob to the JMF */
typedef struct qTexDef_117_s
{
	// w is xShift
	vec4_t m_UAxis;

	// w is yShift
	vec4_t m_VAxis;

	vec2_t m_scale;

	float m_rotate;

	/* See TEXALIGN_ defines above */
	int m_textureAlignment;

	int m_value;		 // Quake II "Value"
	int m_surfaceFlags;	 // Quake II SurfaceFlags
	int m_contentFlags;	 // Quake II ContentFlags

	char m_textureName[64];
} qTexDef_117_t;
COMPILE_TIME_ASSERT( sizeof( qTexDef_117_s ) == 124 );

FORCEINLINE void ConvertTexDef( const qTexDef_117_s &oldTexDef, qTexDef_s &texDef )
{
	texDef.m_UAxis = oldTexDef.m_UAxis;
	texDef.m_VAxis = oldTexDef.m_VAxis;
	texDef.m_scale = oldTexDef.m_scale;
	texDef.m_rotate = oldTexDef.m_rotate;
	texDef.m_textureAlignment = oldTexDef.m_textureAlignment;
	texDef.m_value = oldTexDef.m_value;
	texDef.m_lightmapScale = 0;
	texDef.m_surfaceFlags = oldTexDef.m_surfaceFlags;
	texDef.m_contentFlags = oldTexDef.m_contentFlags;
	memcpy( texDef.m_textureName, oldTexDef.m_textureName, sizeof( texDef.m_textureName ) );
}

/*
===============
JMFSerializer
===============
*/
#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
JMFSerializer::JMFSerializer( const char *filePath, size_t seekOffset, size_t readLimit, struct qWorld_s *worldDef )
	: Serializer( filePath, seekOffset, readLimit, worldDef, FMODE_STDIO_BINARY )
#else
JMFSerializer::JMFSerializer( const char *filePath, struct qWorld_s *worldDef )
	: Serializer( filePath, worldDef, FMODE_STDIO_BINARY )
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA
{
	// worldDef is required for the JMF Serializer
	assert( worldDef );
	assert( worldDef->m_mapWorld );

	m_mapWorld = worldDef->m_mapWorld;

	m_jmfVersion = 0;
}

/*
===============
~JMFSerializer
===============
*/
JMFSerializer::~JMFSerializer()
{
}

/*
===============
Export
===============
*/
bool JMFSerializer::Export()
{
	if ( !m_mapWorld )
	{
		return false;
	}

	if ( !FBitSet( m_mapWorld->m_editorFlags, ( 1 << 23 ) ) )
	{
		Sys_Printf( "Saving: \"%s\"", m_filePath );
	}

	m_jmfVersion = 122;

	if ( !OpenForWrite() )
	{
		return false;
	}

	if ( !ProcessHeader() )
	{
		Sys_Error( "can't write file header" );
		return false;
	}

	// Export info
	{
		// TODO
		int hasExportPath = 0;
		if ( !WriteData( &hasExportPath, sizeof( int ) ) )
		{
			Sys_Error( "can't write export info" );
			return false;
		}
	}

	assert( m_writeMode );

	// Background images
	{
		// TODO: Read bgImage from a list inside m_project
		CBackgroundImage bgImage;
		memset( &bgImage, 0, sizeof( CBackgroundImage ) );

		bgImage.m_filePath = Sys_AllocString( "" );

		for ( int i = 0; i < 3; i++ )
		{
			if ( !SerializeBackground( &bgImage ) )
			{
				return false;
			}
		}
	}

	// Groups
	{
		int groupCount = 0;
		for ( qGroup_s *groupDef = m_mapWorld->m_groupList; groupDef != NULL; groupDef = groupDef->next )
		{
			// Serialize only useful groups
			if ( groupDef->m_objectCount > 0 )
			{
				++groupCount;
			}
		}

		if ( !WriteData( &groupCount, sizeof( int ) ) )
		{
			return false;
		}

		int ignoredGroupCount = 0;
		for ( qGroup_s *groupDef = m_mapWorld->m_groupList; groupDef != NULL; groupDef = groupDef->next )
		{
			// Serialize only useful groups
			if ( groupDef->m_objectCount > 0 )
			{
				if ( !SerializeGroup( groupDef ) )
				{
					return false;
				}
			}
			else
			{
				++ignoredGroupCount;
			}
		}

		if ( ignoredGroupCount != 0 )
		{
			Sys_Printf( "%i empty group(s) ignored", ignoredGroupCount );
		}
	}

	// VisGroups
	{
		int visGroupCount = 0;

		visGroupCount = (int)m_mapWorld->m_visGroupList.size();

		if ( !WriteData( &visGroupCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( auto visGroup : m_mapWorld->m_visGroupList )
		{
			if ( !SerializeVisGroup( visGroup ) )
			{
				return false;
			}
		}
	}

	if ( !WriteData( &m_mapWorld->m_vecCordonMin, sizeof( vec3_t ) ) || !WriteData( &m_mapWorld->m_vecCordonMax, sizeof( vec3_t ) ) )
	{
		return false;
	}

	// Cameras
	{
		int cameraCount = 0;
		for ( qCamera_s *cameraDef = m_mapWorld->m_cameraList; cameraDef != NULL; cameraDef = cameraDef->next )
		{
			++cameraCount;
		}

		if ( !WriteData( &cameraCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( qCamera_s *cameraDef = m_mapWorld->m_cameraList; cameraDef != NULL; cameraDef = cameraDef->next )
		{
			if ( !SerializeCamera( cameraDef ) )
			{
				return false;
			}
		}
	}

	// Paths
	{
		int pathCount = 0;
		for ( qPath_s *pathDef = m_mapWorld->m_pathList; pathDef != NULL; pathDef = pathDef->next )
		{
			++pathCount;
		}

		if ( !WriteData( &pathCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( qPath_s *pathDef = m_mapWorld->m_pathList; pathDef != NULL; pathDef = pathDef->next )
		{
			if ( /*pathDef->m_nodeList != NULL ||*/ !SerializePath( pathDef ) )
			{
				return false;
			}
		}
	}

	// Entities
	{
		for ( qEntity_s *entityDef = m_mapWorld->m_entityList; entityDef != NULL; entityDef = entityDef->next )
		{
			if ( !SerializeEntity( entityDef ) )
			{
				return false;
			}
		}
	}

	return true;
}

/*
===============
Import
===============
*/
bool JMFSerializer::Import()
{
	if ( !m_mapWorld )
	{
		return false;
	}

	if ( !FBitSet( m_mapWorld->m_editorFlags, ( 1 << 23 ) ) )
	{
		Sys_Printf( "Loading: \"%s\"", m_filePath );
	}

	if ( !OpenForRead() )
	{
		return false;
	}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
	fseek( m_fileHandle, 0, SEEK_END );
	m_readLimit = (size_t)ftell( m_fileHandle );
	fseek( m_fileHandle, 0, SEEK_SET );
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

	m_jmfVersion = 0;

	if ( !ProcessHeader() )
	{
		return false;
	}

	if ( m_jmfVersion >= 114 )
	{
		int hasExportPath = 0;
		if ( ReadData( &hasExportPath, sizeof( int ) ) && hasExportPath != 0 )
		{
			char *exportPath = NULL;
			if ( ReadString( &exportPath ) && ( exportPath && exportPath[0] ) )
			{
				// Append to a list inside m_project
			}
		}
	}

	if ( m_jmfVersion >= 122 )
	{
		for ( int i = 0; i < 3; i++ )
		{
			// Use a list inside m_project
			SerializeBackground( NULL );
		}
	}

	int groupCount = 0;
	if ( ReadData( &groupCount, sizeof( int ) ) )
	{
		for ( int i = 0; i < groupCount; i++ )
		{
			SerializeGroup( NULL );
		}
	}

	int visGroupCount = 0;
	if ( ReadData( &visGroupCount, sizeof( int ) ) )
	{
		for ( int i = 0; i < visGroupCount; i++ )
		{
			SerializeVisGroup( NULL );
		}
	}

	if ( !ReadData( &m_worldDef->m_vecCordonMin, sizeof( vec3_t ) ) || !ReadData( &m_worldDef->m_vecCordonMax, sizeof( vec3_t ) ) )
	{
		return false;
	}

	int cameraCount = 0;
	if ( ReadData( &cameraCount, sizeof( int ) ) )
	{
		for ( int i = 0; i < cameraCount; i++ )
		{
			SerializeCamera( NULL );
		}
	}

	int pathCount = 0;
	if ( ReadData( &pathCount, sizeof( int ) ) )
	{
		for ( int i = 0; i < pathCount; i++ )
		{
			SerializePath( NULL );
		}
	}

	while ( SerializeEntity( NULL ) )
		;

	if ( !m_worldDef->m_entityList )
	{
		Sys_Error( "the file is corrupt" );
		return false;
	}

	return true;
}

/*
===============
WriteString
===============
*/
bool JMFSerializer::WriteString( const char *data )
{
	int length = -1;

	if ( data != NULL )
	{
		length = strlen( data );
	}

	bool bOK = WriteData( &length, sizeof( int ) );
	if ( bOK && length > 0 )
	{
		bOK = WriteData( data, length );
	}

	return bOK;
}

/*
===============
ReadString
===============
*/
bool JMFSerializer::ReadString( char **outBuf )
{
	int length = -1;

	if ( !ReadData( &length, sizeof( int ) ) || ( length == -1 ) )
	{
		*outBuf = NULL;
		return false;
	}

	char *string = (char *)Sys_Malloc( length + 1 );
	if ( !string )
	{
		*outBuf = NULL;
		return false;
	}

	if ( length > 0 && !ReadData( string, length ) )
	{
		free( string );
		*outBuf = NULL;
		return false;
	}

	*outBuf = string;
	return true;
}

/*
===============
WriteStringVersion
===============
*/
bool JMFSerializer::WriteStringVersion( const char *data, int jmfVersion )
{
	if ( m_jmfVersion >= jmfVersion )
	{
		return WriteString( data );
	}

	return false;
}

/*
===============
ReadStringVersion
===============
*/
bool JMFSerializer::ReadStringVersion( char **outBuf, int jmfVersion )
{
	if ( m_jmfVersion >= jmfVersion )
	{
		return ReadString( outBuf );
	}

	return false;
}

/*
===============
ProcessHeader
===============
*/
bool JMFSerializer::ProcessHeader()
{
	if ( m_writeMode )
	{
		int magic = JHMFHEADER;
		bool bOK = WriteData( &magic, sizeof( int ) );
		if ( bOK )
		{
			bOK = WriteData( &m_jmfVersion, sizeof( int ) );
		}

		return bOK;
	}
	else
	{
		int magic = 0;
		if ( ReadData( &magic, sizeof( int ) ) && magic == JHMFHEADER )
		{
			if ( ReadData( &m_jmfVersion, sizeof( int ) ) && m_jmfVersion >= 111 )
			{
				return true;
			}

			Sys_Error( "bad file version (%i should be %i or higher)", m_jmfVersion, 111 );
			return false;
		}
	}

	Sys_Error( "bad file header" );
	return false;
}

/*
===============
SerializeBackground
===============
*/
bool JMFSerializer::SerializeBackground( CBackgroundImage *bgImage )
{
	if ( m_writeMode )
	{
		if ( bgImage != NULL )
		{
			if ( !WriteString( bgImage->m_filePath ) )
			{
				return false;
			}

			if ( !WriteData( &bgImage->m_parms, sizeof( CBackgroundImage::Parms ) ) )
			{
				return false;
			}
		}
		else
		{
			if ( !WriteString( NULL ) )
			{
				return false;
			}

			CBackgroundImage::Parms parms;
			memset( &parms, 0, sizeof( CBackgroundImage::Parms ) );
			if ( !WriteData( &parms, sizeof( CBackgroundImage::Parms ) ) )
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		char *filePath = NULL;
		if ( !ReadString( &filePath ) )
		{
			return false;
		}

		if ( bgImage != NULL )
		{
			bgImage->m_filePath = Sys_AllocString( filePath );

			CBackgroundImage::Parms parms {};
			if ( !ReadData( &parms, sizeof( CBackgroundImage::Parms ) ) )
			{
				return false;
			}

			bgImage->setParms( parms );
		}
		else
		{
			CBackgroundImage::Parms parms {};
			if ( !ReadData( &parms, sizeof( CBackgroundImage::Parms ) ) )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

/*
===============
SerializeGroup
===============
*/
bool JMFSerializer::SerializeGroup( qGroup_s *groupDef )
{
	if ( m_writeMode )
	{
		assert( groupDef );

		if ( !FBitSet( groupDef->m_editorFlags, EFL_IGNORE ) )
		{
			int parentEditorId = 0;
			rgba_t editorColor = groupDef->m_mapGroup->m_cbColor1;

			if ( groupDef->m_parent )
			{
				if ( !SerializeGroup( groupDef->m_parent ) )
				{
					return false;
				}

				// This code makes no sense at all.
				// It must get the color from groupDef->m_parent->m_mapGroup and also check if m_mapGroup is not NULL
				editorColor = groupDef->m_mapGroup->m_cbColor1;

				if ( groupDef->m_parent )
				{
					parentEditorId = groupDef->m_parent->m_editorId;
				}
			}

			if ( !WriteData( &groupDef->m_editorId, sizeof( int ) ) ||
				 !WriteData( &parentEditorId, sizeof( int ) ) ||
				 !WriteData( &groupDef->m_editorFlags, sizeof( int ) ) ||
				 !WriteData( &groupDef->m_objectCount, sizeof( int ) ) ||
				 !WriteData( &editorColor, sizeof( rgba_t ) ) )
			{
				return false;
			}

			SetBits( groupDef->m_editorFlags, EFL_IGNORE );
			return true;
		}
	}
	else
	{
		int editorId = 0;
		int parentEditorId = 0;
		int editorFlags = 0;
		int objectCount = 0;
		rgba_t editorColor;

		if ( !ReadData( &editorId, sizeof( int ) ) ||
			 !ReadData( &parentEditorId, sizeof( int ) ) ||
			 !ReadData( &editorFlags, sizeof( int ) ) ||
			 !ReadData( &objectCount, sizeof( int ) ) ||
			 !ReadData( &editorColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		qGroup_s *groupDef = CMapGroup::create();
		m_mapWorld->pushGroup( groupDef );

		groupDef->m_ownerWorld = m_mapWorld;

		groupDef->m_editorFlags = editorFlags;
		groupDef->m_editorId = editorId;
		groupDef->m_objectCount = objectCount;

		groupDef->m_mapGroup->m_cbColor1 = editorColor;
		//groupDef->m_mapGroup->calcFinalColor();

		if ( parentEditorId != 0 )
		{
			for ( qGroup_s *gDef = m_worldDef->m_groupList; gDef != NULL; gDef = gDef->next )
			{
				if ( gDef->m_editorId == parentEditorId )
				{
					groupDef->m_parent = gDef;
					break;
				}
			}
		}

		if ( FBitSet( groupDef->m_editorFlags, EFL_SELECTED ) )
		{
			ClearBits( groupDef->m_editorFlags, EFL_SELECTED );
			SetBits( groupDef->m_editorFlags, EFL_SELECTED );

			pushSelected<qGroup_s>( m_mapWorld->m_selGroupList, groupDef );
		}

		return true;
	}

	return false;
}

/*
===============
SerializeVisGroup
===============
*/
bool JMFSerializer::SerializeVisGroup( CVisGroup *visGroupDef )
{
	if ( m_writeMode )
	{
		assert( visGroupDef );

		if ( !WriteString( visGroupDef->m_name ) ||
			 !WriteData( &visGroupDef->m_editorId, sizeof( int ) ) ||
			 !WriteData( &visGroupDef->m_cbColor, sizeof( rgba_t ) ) ||
			 !WriteData( &visGroupDef->m_visible, sizeof( bool ) ) )
		{
			return false;
		}

		return true;
	}
	else
	{
		char *visGroupName = NULL;
		if ( !ReadString( &visGroupName ) )
		{
			return false;
		}

		CVisGroup *visGroup = m_mapWorld->createVisGroup( visGroupName );

		int editorId = 0;
		rgba_t editorColor = rgba_t( 0, 0, 0, 255 );
		bool isVisible = true;

		if ( !ReadData( &editorId, sizeof( int ) ) ||
			 !ReadData( &editorColor, sizeof( rgba_t ) ) ||
			 !ReadData( &isVisible, sizeof( bool ) ) )
		{
			return false;
		}

		visGroup->m_editorId = editorId;
		visGroup->m_cbColor = editorColor;
		visGroup->m_visible = isVisible;
		return true;
	}

	return false;
}

/*
===============
SerializeCamera
===============
*/
bool JMFSerializer::SerializeCamera( qCamera_s *cameraDef )
{
	if ( m_writeMode )
	{
		assert( cameraDef );

		if ( !WriteData( &cameraDef->m_vecOrigin, sizeof( vec3_t ) ) ||
			 !WriteData( &cameraDef->m_vecAngles, sizeof( vec3_t ) ) ||
			 !WriteData( &cameraDef->m_editorFlags, sizeof( int ) ) ||
			 !WriteData( &cameraDef->m_mapCamera->m_cbColor1, sizeof( rgba_t ) ) )
		{
			return false;
		}

		return true;
	}
	else
	{
		vec3_t origin;
		vec3_t angles;
		int editorFlags = 0;
		rgba_t editorColor;

		if ( !ReadData( &origin, sizeof( vec3_t ) ) ||
			 !ReadData( &angles, sizeof( vec3_t ) ) ||
			 !ReadData( &editorFlags, sizeof( int ) ) ||
			 !ReadData( &editorColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		qCamera_s *cameraDef = CMapCamera::create();
		cameraDef->m_ownerWorld = m_mapWorld;

		//cameraDef->m_mapCamera->m_selectionId = m_mapWorld->alloc_selectionId();

		push<qCamera_s>( m_mapWorld->m_cameraList, m_worldDef->m_lastCamera, cameraDef );

		SetBits( cameraDef->m_editorFlags, editorFlags );

		GetNewEditorId( cameraDef, m_mapWorld->m_lastUsedCameraEditorId );

		cameraDef->m_mapCamera->setup( origin, angles );

		cameraDef->m_mapCamera->m_cbColor1 = editorColor;
		//cameraDef->m_mapCamera->calcFinalColor();

		if ( FBitSet( cameraDef->m_editorFlags, EFL_SELECTED ) )
		{
			m_worldDef->m_selCamera = cameraDef;
		}

		return true;
	}

	return false;
}

/*
===============
SerializePath
===============
*/
bool JMFSerializer::SerializePath( qPath_s *pathDef )
{
	if ( m_writeMode )
	{
		assert( pathDef );

		if ( !WriteString( pathDef->m_pathClassname ) ||
			 !WriteString( pathDef->m_pathName ) ||
			 !WriteData( &pathDef->m_pathDirection, sizeof( int ) ) ||
			 !WriteData( &pathDef->m_editorFlags, sizeof( int ) ) ||
			 !WriteData( &pathDef->m_mapPath->m_cbColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		int nodeCount = 0;
		for ( qNode_s *nodeDef = pathDef->m_nodeList; nodeDef != NULL; nodeDef = nodeDef->next )
		{
			++nodeCount;
		}

		if ( !WriteData( &nodeCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( qNode_s *nodeDef = pathDef->m_nodeList; nodeDef != NULL; nodeDef = nodeDef->next )
		{
			SerializeNode( nodeDef, pathDef );
		}

		return true;
	}
	else
	{
		char *classname = NULL;
		ReadString( &classname );

		if ( !classname || !classname[0] )
		{
			return false;
		}

		char *name = NULL;
		ReadString( &name );

		int direction = 0;
		int editorFlags = 0;
		rgba_t editorColor;

		if ( !ReadData( &direction, sizeof( int ) ) || !ReadData( &editorFlags, sizeof( int ) ) || !ReadData( &editorColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		qPath_t *pathDef = CMapPath::create();
		pathDef->m_ownerWorld = m_mapWorld;

		pathDef->m_mapPath->m_cbColor = editorColor;

		push<qPath_s>( m_mapWorld->m_pathList, m_mapWorld->m_lastPath, pathDef );

		GetNewEditorId( pathDef, m_mapWorld->m_lastUsedPathEditorId );

		pathDef->m_pathClassname = classname;
		pathDef->m_pathName = name;
		pathDef->m_pathDirection = direction;
		pathDef->m_editorFlags = editorFlags;

		int nodeCount = 0;
		if ( !ReadData( &nodeCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( int i = 0; i < nodeCount; i++ )
		{
			SerializeNode( NULL, pathDef );
		}

#if JACK_API_VERSION >= API_VERSION_STEAM_BETA
		int nodeIdx = 1;
		for ( qNode_s *nodeDef = pathDef->m_nodeList; nodeDef != NULL; nodeDef = nodeDef->next )
		{
			if ( nodeDef )
			{
				nodeDef->m_nodeIndex = nodeIdx;
			}

			++nodeIdx;
		}
#endif // JACK_API_VERSION >= API_VERSION_STEAM_BETA

		//pathDef->m_mapPath->build( 0 );
		pathDef->m_editorFlags &= ~0xFFFDFFFD; // TODO

		return true;
	}

	return false;
}

/*
===============
SerializeNode
===============
*/
bool JMFSerializer::SerializeNode( qNode_s *nodeDef, qPath_s *pathDef )
{
	if ( m_writeMode )
	{
		assert( nodeDef );
		assert( pathDef );

		if ( !WriteString( nodeDef->m_nameOverride ) ||
			 !WriteString( nodeDef->m_fireEntityOnPass ) ||
			 !WriteData( &nodeDef->m_vecOrigin, sizeof( vec3_t ) ) ||
			 !WriteData( &nodeDef->m_vecAngles, sizeof( vec3_t ) ) ||
			 !WriteData( &nodeDef->m_editorFlags, sizeof( int ) ) ||
			 !WriteData( &nodeDef->m_mapNode->m_cbColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		int keyValueCount = 0;
		for ( epair_s *epair = nodeDef->epairs; epair != NULL; epair = epair->next )
		{
			++keyValueCount;
		}

		if ( !WriteData( &keyValueCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( epair_s *epair = nodeDef->epairs; epair != NULL; epair = epair->next )
		{
			if ( !WriteString( epair->key ) || !WriteString( epair->value ) )
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		qNode_s *nodeDef = CMapNode::create();

		//nodeDef->m_mapNode->m_selectionId = m_mapWorld->alloc_selectionId();

		push<qNode_s>( pathDef->m_nodeList, pathDef->m_lastNode, nodeDef );

		nodeDef->m_ownerPath = pathDef;

		GetNewEditorId( nodeDef, m_mapWorld->m_lastUsedNodeEditorId );

		ReadString( (char **)&nodeDef->m_nameOverride );
		ReadString( (char **)&nodeDef->m_fireEntityOnPass );

		int editorFlags = 0;
		rgba_t editorColor;

		if ( !ReadData( &nodeDef->m_vecOrigin, sizeof( vec3_t ) ) ||
			 !ReadData( &nodeDef->m_vecAngles, sizeof( vec3_t ) ) ||
			 !ReadData( &editorFlags, sizeof( int ) ) ||
			 !ReadData( &editorColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		SetBits( nodeDef->m_editorFlags, editorFlags );
		nodeDef->m_mapNode->m_cbColor = editorColor;

		int keyValueCount = 0;
		if ( !ReadData( &keyValueCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( int i = 0; i < keyValueCount; i++ )
		{
			char *key = NULL;
			char *value = NULL;

			ReadString( &key );
			ReadString( &value );

			if ( key != NULL && value != NULL )
			{
				AppendEpair( &nodeDef->epairs, key, value );
			}
		}

		ClearBits( nodeDef->m_editorFlags, EFL_SELECTED );
		return true;
	}

	return false;
}

/*
===============
SerializeEntity
===============
*/
bool JMFSerializer::SerializeEntity( qEntity_s *entityDef )
{
	if ( m_writeMode )
	{
		assert( entityDef );

		int groupId = 0;
		int parentGroupId = 0;

		if ( entityDef->m_groupList != NULL )
		{
			groupId = entityDef->m_groupList->m_editorId;
		}

		if ( entityDef->m_lastGroup != NULL )
		{
			parentGroupId = entityDef->m_lastGroup->m_editorId;
		}

		if ( !WriteString( entityDef->m_className ) ||
			 !WriteData( &entityDef->m_vecOrigin, sizeof( vec3_t ) ) ||
			 !WriteData( &entityDef->m_editorFlags, sizeof( int ) ) ||
			 !WriteData( &groupId, sizeof( int ) ) ||
			 !WriteData( &parentGroupId, sizeof( int ) ) ||
			 !WriteData( &entityDef->m_mapEntity->m_cbColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		// "spawnflags"
		if ( !WriteString( entityDef->m_entityKeys[0].key ) )
		{
			return false;
		}

		// "origin" "angles" "scale"
		if ( !WriteStringVersion( entityDef->m_entityKeys[1].key, 113 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[2].key, 113 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[3].key, 113 ) )
		{
			return false;
		}

		// "targetname" "target"
		if ( !WriteStringVersion( entityDef->m_entityKeys[4].key, 116 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[5].key, 116 ) )
		{
			return false;
		}

		// "skyname"
		if ( !WriteStringVersion( entityDef->m_entityKeys[6].key, 117 ) )
		{
			return false;
		}

		// "model" "model" "texture" "model" "model"
		if ( !WriteStringVersion( entityDef->m_entityKeys[7].key, 119 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[8].key, 119 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[9].key, 119 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[10].key, 119 ) ||
			 !WriteStringVersion( entityDef->m_entityKeys[11].key, 119 ) )
		{
			return false;
		}

		// "script"
		if ( !WriteStringVersion( entityDef->m_entityKeys[12].key, 121 ) )
		{
			return false;
		}

		if ( !WriteData( &entityDef->m_spawnflags, sizeof( int ) ) ||
			 !WriteData( &entityDef->m_vecAngles, sizeof( vec3_t ) ) ||
			 !WriteData( &entityDef->m_renderMode, sizeof( qEntityRenderMode_s ) ) )
		{
			return false;
		}

		qEntityState_s &entityState = entityDef->m_entityState;
		if ( m_jmfVersion <= 114 )
		{
			if ( !WriteData( &entityState, 16 ) )
			{
				return false;
			}
		}
		else
		{
			if ( !WriteData( &entityState, sizeof( qEntityState_s ) ) )
			{
				return false;
			}
		}

		int keyValueCount = 0;
		for ( epair_s *epair = entityDef->epairs; epair != NULL; epair = epair->next )
		{
			++keyValueCount;
		}

		if ( !WriteData( &keyValueCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( epair_s *epair = entityDef->epairs; epair != NULL; epair = epair->next )
		{
			if ( !WriteString( epair->key ) || !WriteString( epair->value ) )
			{
				return false;
			}
		}

		// TODO
		int visGroupCount = 0;
		if ( !WriteData( &visGroupCount, sizeof( int ) ) )
		{
			return false;
		}

		int brushCount = 0;
		for ( qBrush_s *brushDef = entityDef->m_brushList; brushDef != NULL; brushDef = brushDef->next )
		{
			++brushCount;
		}

		if ( !WriteData( &brushCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( qBrush_s *brushDef = entityDef->m_brushList; brushDef != NULL; brushDef = brushDef->next )
		{
			SerializeBrush( brushDef, entityDef );
		}

		return true;
	}
	else
	{
		char *classname = NULL;
		ReadString( &classname );

		if ( !classname || !classname[0] )
		{
			return false;
		}

		vec3_t origin;
		int editorFlags = 0;

		int groupId = 0;
		int parentGroupId = 0;

		rgba_t editorColor;

		if ( !ReadData( &origin, sizeof( vec3_t ) ) ||
			 !ReadData( &editorFlags, sizeof( int ) ) ||
			 !ReadData( &groupId, sizeof( int ) ) ||
			 !ReadData( &parentGroupId, sizeof( int ) ) ||
			 !ReadData( &editorColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		qEntity_s *entityDef = CMapEntity::create( classname, origin, editorFlags );
		entityDef->m_ownerWorld = m_mapWorld;

		entityDef->m_mapEntity->m_cbColor = editorColor;

		//m_mapWorld->linkObjectToWorld( entityDef );

		push<qEntity_s>( m_mapWorld->m_entityList, m_mapWorld->m_lastEntity, entityDef );

		GetNewEditorId( entityDef, m_mapWorld->m_lastUsedEntityEditorId );

		ReadString( (char **)&entityDef->m_entityKeys[0].key );			 // "spawnflags"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[1], 113 );	 // "origin"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[2], 113 );	 // "angles"
		// askForVersion113BugWorkaround
		ReadStringVersion( (char **)&entityDef->m_entityKeys[3], 113 );	 // "scale"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[4], 116 );	 // "targetname"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[5], 116 );	 // "target"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[6], 117 );	 // "skyname"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[7], 119 );	 // "model"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[8], 119 );	 // "model"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[9], 119 );	 // "texture"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[10], 119 ); // "model"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[11], 119 ); // "model"
		ReadStringVersion( (char **)&entityDef->m_entityKeys[12], 121 ); // "script"

		if ( !ReadData( &entityDef->m_spawnflags, sizeof( int ) ) || !ReadData( &entityDef->m_vecAngles, sizeof( vec3_t ) ) || !ReadData( &entityDef->m_renderMode, sizeof( qEntityRenderMode_s ) ) )
		{
			return false;
		}

		qEntityState_s &entityState = entityDef->m_entityState;
		if ( m_jmfVersion <= 114 )
		{
			if ( !ReadData( &entityState, 16 ) )
			{
				return false;
			}
		}
		else
		{
			if ( !ReadData( &entityState, sizeof( qEntityState_s ) ) )
			{
				return false;
			}
		}

		int keyValueCount = 0;
		if ( !ReadData( &keyValueCount, sizeof( int ) ) || keyValueCount > 32000 )
		{
			return false;
		}

		for ( int i = 0; i < keyValueCount; i++ )
		{
			char *key = NULL;
			char *value = NULL;

			ReadString( &key );
			ReadString( &value );

			if ( key != NULL && value != NULL )
			{
				AppendEpair( &entityDef->epairs, key, value );
			}
		}

		int visGroupCount = 0;
		if ( !ReadData( &visGroupCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( int i = 0; i < visGroupCount; i++ )
		{
			int visGroupId = 0;
			if ( !ReadData( &visGroupId, sizeof( int ) ) )
			{
				return false;
			}

			// CMapEntity::addToVisGroup
		}

		int brushCount = 0;
		if ( !ReadData( &brushCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( int i = 0; i < brushCount; i++ )
		{
			SerializeBrush( NULL, entityDef );
		}

		//entityDef->m_mapEntity->updateKeys( true );
		//entityDef->m_mapEntity->build( 906 );

		if ( groupId > 0 )
		{
			for ( qGroup_s *groupDef = m_mapWorld->m_groupList; groupDef != NULL; groupDef = groupDef->next )
			{
				if ( groupId == groupDef->m_editorId )
				{
					entityDef->m_groupList = groupDef;
					break;
				}
			}
		}

		if ( parentGroupId > 0 )
		{
			for ( qGroup_s *groupDef = m_mapWorld->m_groupList; groupDef != NULL; groupDef = groupDef->next )
			{
				if ( parentGroupId == groupDef->m_editorId )
				{
					entityDef->m_lastGroup = groupDef;
					break;
				}
			}
		}

		if ( FBitSet( entityDef->m_editorFlags, EFL_SELECTED ) )
		{
			ClearBits( entityDef->m_editorFlags, EFL_SELECTED );
			SetBits( entityDef->m_editorFlags, EFL_SELECTED );

			pushSelected<qEntity_s>( m_mapWorld->m_selEntityList, entityDef );

			//entityDef->m_mapEntity->updateSplineParms( 32 );
		}

		return true;
	}

	return false;
}

/*
===============
SerializeBrush
===============
*/
bool JMFSerializer::SerializeBrush( qBrush_s *brushDef, qEntity_s *entityDef )
{
	if ( m_writeMode )
	{
		assert( brushDef );
		assert( entityDef );

		int groupId = 0;
		int parentGroupId = 0;

		if ( entityDef->m_groupList != NULL )
		{
			groupId = entityDef->m_groupList->m_editorId;
		}

		if ( entityDef->m_lastGroup != NULL )
		{
			parentGroupId = entityDef->m_lastGroup->m_editorId;
		}

		if ( !WriteData( &brushDef->m_extraDataType, sizeof( int ) ) ||
			 !WriteData( &brushDef->m_editorFlags, sizeof( int ) ) ||
			 !WriteData( &groupId, sizeof( int ) ) ||
			 !WriteData( &parentGroupId, sizeof( int ) ) ||
			 !WriteData( &brushDef->m_mapBrush->m_cbColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		// TODO
		int visGroupCount = 0;
		if ( !WriteData( &visGroupCount, sizeof( int ) ) )
		{
			return false;
		}

		int faceCount = 0;
		for ( qFace_s *faceDef = brushDef->m_faceList; faceDef != NULL; faceDef = faceDef->next )
		{
			++faceCount;
		}

		if ( !WriteData( &faceCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( qFace_s *faceDef = brushDef->m_faceList; faceDef != NULL; faceDef = faceDef->next )
		{
			SerializeFace( faceDef, brushDef );
		}

		switch ( brushDef->m_extraDataType )
		{
		case BRUSH_EXTRADATA_PATCH:
		{
			assert( brushDef->m_patch );
			qPatch_s *patchDef = brushDef->m_patch;

			if ( !WriteData( &patchDef->m_numColumns, sizeof( int ) ) ||
				 !WriteData( &patchDef->m_numRows, sizeof( int ) ) ||
				 !WriteData( &patchDef->m_texDef, sizeof( qTexDef_s ) ) ||
				 !WriteData( &patchDef->m_editorFlags, sizeof( int ) ) ||
				 !WriteData( &patchDef->m_data, sizeof( patchDef->m_data ) ) )
			{
				return false;
			}

			break;
		}
		case BRUSH_EXTRADATA_OVERLAY:
		{
			assert( brushDef->m_overlay );
			qOverlay_s *overlayDef = brushDef->m_overlay;

			if ( !WriteData( &overlayDef->m_texDef, sizeof( qTexDef_s ) ) ||
				 !WriteData( &overlayDef->m_editorFlags, sizeof( int ) ) ||
				 !WriteData( &overlayDef->m_plane, sizeof( qPlane_s ) ) ||
				 !WriteData( &overlayDef->m_data, sizeof( overlayDef->m_data ) ) )
			{
				return false;
			}

			break;
		}
		}

		return true;
	}
	else
	{
		int extraDataType = 0;
		int editorFlags = 0;
		int groupId = 0;
		int parentGroupId = 0;
		rgba_t editorColor;

		if ( m_jmfVersion > 111 && !ReadData( &extraDataType, sizeof( int ) ) ||
			 !ReadData( &editorFlags, sizeof( int ) ) ||
			 !ReadData( &groupId, sizeof( int ) ) ||
			 !ReadData( &parentGroupId, sizeof( int ) ) ||
			 !ReadData( &editorColor, sizeof( rgba_t ) ) )
		{
			return false;
		}

		qBrush_s *brushDef = CMapBrush::create();

		SetBits( brushDef->m_editorFlags, editorFlags );

		brushDef->m_ownerEntity = entityDef;

		brushDef->m_extraDataType = extraDataType;

		brushDef->m_mapBrush->m_cbColor = editorColor;
		//brushDef->m_mapBrush->calcFinalColor();

		//brushDef->m_mapBrush->m_selectionId = m_mapWorld->alloc_selectionId();

		push<qBrush_s>( entityDef->m_brushList, entityDef->m_lastBrush, brushDef );

		GetNewEditorId( brushDef, m_mapWorld->m_lastUsedBrushEditorId );

		int visGroupCount = 0;
		if ( !ReadData( &visGroupCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( int i = 0; i < visGroupCount; i++ )
		{
			int visGroupId = 0;
			if ( !ReadData( &visGroupId, sizeof( int ) ) )
			{
				return false;
			}

			//brushDef->m_mapbrush->addToVisGroup( visGroup );
		}

		int faceCount = 0;
		if ( !ReadData( &faceCount, sizeof( int ) ) )
		{
			return false;
		}

		for ( int i = 0; i < faceCount; i++ )
		{
			SerializeFace( NULL, brushDef );
		}

		switch ( extraDataType )
		{
		case BRUSH_EXTRADATA_PATCH:
		{
			int numColumns = 0;
			int numRows = 0;

			if ( !ReadData( &numColumns, sizeof( int ) ) || !ReadData( &numRows, sizeof( int ) ) )
			{
				return false;
			}

			qPatch_t *patchDef = CMapPatch::create( numColumns, numRows );

			qTexDef_s texDef;
			if ( m_jmfVersion > 117 )
			{
				if ( !ReadData( &texDef, sizeof( qTexDef_s ) ) )
				{
					return false;
				}
			}
			else
			{
				qTexDef_117_s oldTexDef;
				if ( !ReadData( &oldTexDef, sizeof( qTexDef_117_s ) ) )
				{
					return false;
				}

				ConvertTexDef( oldTexDef, texDef );
			}

			if ( !ReadData( &patchDef->m_editorFlags, sizeof( int ) ) )
			{
				return false;
			}

			if ( m_jmfVersion > 119 )
			{
				if ( !ReadData( &patchDef->m_data, sizeof( patchDef->m_data ) ) )
				{
					return false;
				}
			}
			else
			{
				Sys_Error( "NOT IMPLEMENTED" );
				return false;
			}

			patchDef->m_ownerBrush = brushDef;
			brushDef->m_patch = patchDef;

			patchDef->m_mapPatch->setTexDef( texDef );
			//patchDef->m_mapPatch->build( 2048 );
			break;
		}
		case BRUSH_EXTRADATA_OVERLAY:
		{
			qOverlay_t *overlayDef = CMapOverlay::create();

			qTexDef_s texDef;
			if ( m_jmfVersion > 117 )
			{
				if ( !ReadData( &texDef, sizeof( qTexDef_s ) ) )
				{
					return false;
				}
			}
			else
			{
				qTexDef_117_s oldTexDef;
				if ( !ReadData( &oldTexDef, sizeof( qTexDef_117_s ) ) )
				{
					return false;
				}

				ConvertTexDef( oldTexDef, texDef );
			}

			if ( !ReadData( &overlayDef->m_editorFlags, sizeof( int ) ) ||
				 !ReadData( &overlayDef->m_plane, sizeof( qPlane_s ) ) ||
				 !ReadData( &overlayDef->m_data, sizeof( overlayDef->m_data ) ) )
			{
				return false;
			}

			overlayDef->m_ownerBrush = brushDef;
			brushDef->m_overlay = overlayDef;

			overlayDef->m_mapOverlay->setTexDef( texDef );
			//overlayDef->m_mapOverlay->build( 0 );
			break;
		}
		}

		//brushDef->m_mapBrush->build( 10 );

		if ( groupId > 0 )
		{
			for ( qGroup_s *groupDef = m_mapWorld->m_groupList; groupDef != NULL; groupDef = groupDef->next )
			{
				if ( groupId == groupDef->m_editorId )
				{
					brushDef->m_groupList = groupDef;
					break;
				}
			}
		}

		if ( parentGroupId > 0 )
		{
			for ( qGroup_s *groupDef = m_mapWorld->m_groupList; groupDef != NULL; groupDef = groupDef->next )
			{
				if ( parentGroupId == groupDef->m_editorId )
				{
					brushDef->m_lastGroup = groupDef;
					break;
				}
			}
		}

		if ( FBitSet( brushDef->m_editorFlags, EFL_SELECTED ) )
		{
			ClearBits( brushDef->m_editorFlags, EFL_SELECTED );
			SetBits( brushDef->m_editorFlags, EFL_SELECTED );

			pushSelected<qBrush_s>( m_mapWorld->m_selBrushList, brushDef );
		}

		return true;
	}

	return false;
}

/*
===============
SerializeFace
===============
*/
bool JMFSerializer::SerializeFace( qFace_s *faceDef, qBrush_s *brushDef )
{
	if ( m_writeMode )
	{
		assert( faceDef );
		assert( brushDef );

		if ( !WriteData( &faceDef->m_editorFlags, sizeof( int ) ) ||
			 !WriteData( &faceDef->m_vertexCount, sizeof( int ) ) ||
			 !WriteData( &faceDef->m_texDef, sizeof( qTexDef_s ) ) ||
			 !WriteData( &faceDef->m_plane, sizeof( qPlane_s ) ) ||
			 !WriteData( &faceDef->m_vertices, faceDef->m_vertexCount * sizeof( qVertex_s ) ) )
		{
			return false;
		}

		return true;
	}
	else
	{
		int editorFlags = 0;
		int vertexCount = 0;
		if ( !ReadData( &editorFlags, sizeof( int ) ) ||
			 !ReadData( &vertexCount, sizeof( int ) ) )
		{
			return false;
		}

		qFace_s *faceDef = CMapFace::create( vertexCount );

		SetBits( faceDef->m_editorFlags, editorFlags );
		faceDef->m_ownerBrush = brushDef;

		qTexDef_s texDef;
		if ( m_jmfVersion > 117 )
		{
			if ( !ReadData( &texDef, sizeof( qTexDef_s ) ) )
			{
				return false;
			}
		}
		else
		{
			qTexDef_117_s oldTexDef;
			if ( !ReadData( &oldTexDef, sizeof( qTexDef_117_s ) ) )
			{
				return false;
			}

			ConvertTexDef( oldTexDef, texDef );
		}

		if ( !ReadData( &faceDef->m_plane, sizeof( qPlane_s ) ) )
		{
			return false;
		}

		if ( !ReadData( faceDef->m_vertices, vertexCount * sizeof( qVertex_s ) ) )
		{
			return false;
		}

		//faceDef->m_mapFace->m_selectionId = m_mapWorld->alloc_selectionId();

		push<qFace_s>( brushDef->m_faceList, brushDef->m_lastFace, faceDef );

		GetNewEditorId( faceDef, m_mapWorld->m_lastUsedFaceEditorId );

		faceDef->m_mapFace->setTexDef( texDef );
		//faceDef->m_mapFace->build( 16 );

		ClearBits( faceDef->m_editorFlags, EFL_SELECTED );

		return true;
	}

	return false;
}
