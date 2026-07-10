//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINGROUPS_H )
#define PLUGINGROUPS_H

/*
 Groups API
*/

#include "BaseTypes.h"

struct qWorld_s;


class CMapGroup;

typedef struct qGroup_s
{
	void *firstPtr;

	/* Internal */
	CMapGroup *m_mapGroup;

	/* Next/Previous face list. First element never has ->prev set and last element never has ->next set */
	struct qGroup_s *next;
	struct qGroup_s *prev;

	/* Next/Previous selected face list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qGroup_s *selectedNext;
	struct qGroup_s *selectedPrev;

	char gap2[8];

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	int m_editorFlags;
	int m_editorId;

	int m_objectCount;

	rgba_t m_editorColor;
} qGroup_t;
COMPILE_TIME_ASSERT( sizeof( qGroup_t ) == 80 );

// clang-format off

// clang-format on

#endif // !PLUGINGROUPS_H
