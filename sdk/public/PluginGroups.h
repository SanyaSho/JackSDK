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

 Groups are stored as a linked list inside of qWorld_s and can be iterated using ->next and ->prev pointers.
*/

#include "BaseTypes.h"

struct qWorld_s;


class CMapGroup;

typedef struct qGroup_s
{
	void *firstPtr;

	/* Internal */
	CMapGroup *m_mapGroup;

	/* Next/Previous group list. First element never has ->prev set and last element never has ->next set */
	struct qGroup_s *next;
	struct qGroup_s *prev;

	/* Next/Previous selected group list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qGroup_s *selectedNext;
	struct qGroup_s *selectedPrev;

	struct qGroup_s *m_parent; // TODO: Recheck

	/* World this entity belongs to */
	struct qWorld_s *m_ownerWorld;

	int m_editorFlags;
	int m_editorId;

	/* Number of objects in this group */
	int m_objectCount;

	/* Color of the group used by the editor */
	rgba_t m_editorColor;
} qGroup_t;
COMPILE_TIME_ASSERT( sizeof( qGroup_t ) == 80 );

// clang-format off

// clang-format on

#endif // !PLUGINGROUPS_H
