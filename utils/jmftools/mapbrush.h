//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPBRUSH_H )
#define MAPBRUSH_H

#include <vector>

#include "PluginAPI.h"
#include "PluginWorld.h"

class CVisGroup;

class CMapOverlay;
class CMapPatch;

class CMapBrush
{
public:
	CMapBrush();
	virtual ~CMapBrush();

	static qBrush_s *create()
	{
		CMapBrush *mapBrush = new CMapBrush;
		mapBrush->m_brushData->m_editorId = 0;
		mapBrush->m_brushData->m_extraDataType = 0;
		SetBits( mapBrush->m_brushData->m_editorFlags, EFL_DIRTY );
		return mapBrush->m_brushData;
	}

public:
	void addToVisGroup( CVisGroup *visGroup );
	void removeFromAllVisGroups();

public:
	qBrush_t *m_brushData;
	int m_selectionId;
	rgba_t m_cbColor;
	rgba_t m_cbFaceColor;
	vec3_t m_brushSize;
	std::vector<CVisGroup *> m_visGroupList;
};

class CMapOverlay
{
public:
	CMapOverlay();
	virtual ~CMapOverlay();

	void setTexDef( const qTexDef_s &texDef );

	static qOverlay_s *create()
	{
		CMapOverlay *mapOverlay = new CMapOverlay;
		SetBits( mapOverlay->m_overlayData->m_editorFlags, EFL_DIRTY );
		return mapOverlay->m_overlayData;
	}

public:
	qOverlay_s *m_overlayData;
	vec3_t m_unknownVec1;
	float m_unkFlt1;
};

class CMapPatch
{
public:
	CMapPatch( int numColumns, int numRows );
	virtual ~CMapPatch();

	void setTexDef( const qTexDef_s &texDef );

	static qPatch_s *create( int numColumns, int numRows )
	{
		CMapPatch *mapPatch = new CMapPatch( numColumns, numRows );
		SetBits( mapPatch->m_patchData->m_editorFlags, EFL_DIRTY );
		return mapPatch->m_patchData;
	}

public:
	qPatch_s *m_patchData;
	char gap[16];
};

#endif // !MAPBRUSH_H
