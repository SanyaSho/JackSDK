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
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapPath *m_mapPath;

	/* Next/Previous path list. First element never has ->prev set and last element never has ->next set */
	struct qPath_s *next;
	struct qPath_s *prev;

	/* List of nodes attached to the path */
	struct qNode_s *m_nodeList;
	struct qNode_s *m_lastNode;

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	char *m_pathName;
	char *m_pathClassname;
	int m_pathDirection;

	int m_editorFlags;
	int m_editorId;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

#if defined( JACK_64BIT )
	char gap4[4];
#endif // JACK_64BIT
} qPath_t;
COMPILE_TIME_ASSERT( sizeof( qPath_t ) == SIZEOF_QPATH_S );

// clang-format off

// clang-format on

#endif // !PLUGINPATHS_H
