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

	static qPath_t *create()
	{
		CMapPath *mapPath = new CMapPath;
		return mapPath->m_pathData;
	}

public:
	qPath_t *m_pathData;
	rgba_t m_cbColor;
	bool m_unknown;
};

class CMapNode
{
public:
	CMapNode();
	virtual ~CMapNode();

	static qNode_t *create()
	{
		CMapNode *mapNode = new CMapNode;
		return mapNode->m_nodeData;
	}

protected:
	void clearEpairs() { CMapEntity::clearEpairs( &m_nodeData->epairs ); }

public:
	qNode_t *m_nodeData;
	int m_selectionId;
	rgba_t m_cbColor;
};

#endif // !MAPPATH_H
