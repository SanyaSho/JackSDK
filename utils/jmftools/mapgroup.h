//=====================================================================================//
//
// Purpose: JMFTools
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( MAPGROUP_H )
#define MAPGROUP_H

#include "PluginGroups.h"

class CMapGroup
{
public:
	CMapGroup();
	virtual ~CMapGroup();

	static qGroup_s *create()
	{
		CMapGroup *mapGroup = new CMapGroup;
		return mapGroup->m_groupData;
	}

public:
	qGroup_t *m_groupData;
	int m_selectionId;
	rgba_t m_cbColor1;
	rgba_t m_cbColor2;
};

#endif // !MAPGROUP_H
