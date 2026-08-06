//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPWORLD_H )
#define MAPWORLD_H

#include <vector>

#include "PluginWorld.h"

struct CGameConfig;
class CProject;
class COctree;

class CVisGroup;

class CMapWorld : public qWorld_s
{
public:
	CMapWorld();
	~CMapWorld();

	CVisGroup *createVisGroup( const char *visGroupName );

	void pushGroup( qGroup_s *groupDef );

	bool buildPackageList( char **outBuf, bool listSeparator, int removeVolumePrefix );

public:
	void clear();
	void removeAllEntities();
	void removeAllVisGroups();
	void removeAllCameras();
	void removeAllPaths();

public:
	CGameConfig *m_gameConfig;
	CProject *m_project;
	COctree *m_octree;

	char CMapWorld_gap[24];
	int m_lastUsedEntityEditorId;
	int m_lastUsedBrushEditorId;
	int m_lastUsedFaceEditorId;
	int m_lastUsedNodeEditorId;
	int m_lastUsedPathEditorId;
	int m_lastUsedCameraEditorId;
	char CMapWorld_gap3[4];
	int m_lastUsedVisGroupEditorId;
	char CMapWorld_gap4[48];

	std::vector<CVisGroup *> m_visGroupList;
};

template <typename T>
void push( T *&linkedList, T *&linkedListLast, T *objectDef )
{
	objectDef->next = NULL;
	objectDef->prev = linkedListLast;
	if ( linkedListLast != NULL )
	{
		linkedListLast->next = objectDef;
	}
	if ( linkedList == NULL )
	{
		linkedList = objectDef;
	}
	linkedListLast = objectDef;
}

template <typename T>
void pushSelected( T *&linkedList, T *objectDef )
{
	objectDef->selectedNext = NULL;
	objectDef->selectedPrev = linkedList;
	if ( linkedList != NULL )
	{
		linkedList->selectedNext = objectDef;
	}
	linkedList = objectDef;
}

#define GetNewEditorId( object, id ) if ( id == -1 ) id = 1; else ++id; object->m_editorId = id;

class CVisGroup
{
public:
	CVisGroup();
	~CVisGroup();

	void setName( const char *name );

public:
	char *m_name;
	rgba_t m_cbColor;
	int m_editorId;
	int gap;
	bool m_visible;
};


class CBackgroundImage
{
public:
	struct Parms
	{
		Parms()
		{
			m_scale = 1.0;
			m_alpha = 255;
			m_filter = 1;
			m_flags = 0;
			m_xOffset = 0;
			m_yOffset = 0;
		}

		double m_scale;
		int m_alpha;
		int m_filter;
		int m_flags;
		int m_xOffset;
		int m_yOffset;
	};

public:
	CBackgroundImage();
	~CBackgroundImage();

	void setParms( const CBackgroundImage::Parms &parms ) { m_parms = parms; }

	char *m_filePath;
	Parms m_parms;
	/*char gap2[8];
	int m_glTextureId;
	bool m_loaded;*/
};

#endif // !MAPWORLD_H
