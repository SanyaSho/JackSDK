//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINPATHS_H )
#define PLUGINPATHS_H

/*
 Paths API
*/

#include "BaseTypes.h"

class CMapPath;

typedef struct qPath_s
{
	void *firstPtr;

	/* Internal */
	CMapPath *m_mapPath;

	/* Next/Previous path list. First element never has ->prev set and last element never has ->next set */
	struct qPath_s *next;
	struct qPath_s *prev;

	struct qNode_s *m_nodeList;
	struct qNode_s *m_lastNode;

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	const char *m_pathName;
	const char *m_pathClassname;
	int m_pathDirection;
	int m_editorFlags;
	int m_editorId;
	vec3_t m_bboxMin;
	vec3_t m_bboxMax;
	char gap4[4];
} qPath_t;
COMPILE_TIME_ASSERT( sizeof( qPath_t ) == 112 );

// clang-format off

// clang-format on

#endif // !PLUGINPATHS_H
