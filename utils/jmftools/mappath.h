//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPPATH_H )
#define MAPPATH_H

#include "PluginPaths.h"
#include "PluginNodes.h"

#include "mapentity.h"

class CMapNode;

class CMapPath
{
public:
	CMapPath();
	virtual ~CMapPath();

	void build( int buildFlags = 0 );

public:
	static qPath_t *create()
	{
		CMapPath *mapPath = new CMapPath;
		return mapPath->m_pathData;
	}

public:
	qPath_t *m_pathData;
	rgba_t m_cbColor;
	bool m_spline;
};

class CMapNode
{
public:
	CMapNode();
	virtual ~CMapNode();

	void updateParms();

public:
	static qNode_t *create()
	{
		CMapNode *mapNode = new CMapNode;
		return mapNode->m_nodeData;
	}

protected:
	void clearEpairs() { CMapEntity::clearEpairs( &m_nodeData->epairs ); }

	epair_s *findEpairByKey( const char *key );

public:
	qNode_t *m_nodeData;
	int m_selectionId;
	rgba_t m_cbColor;
};

#endif // !MAPPATH_H
