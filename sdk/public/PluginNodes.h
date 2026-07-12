//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINNODES_H )
#define PLUGINNODES_H

/*
 Nodes API
*/

#include "BaseTypes.h"

class CMapNode;

typedef struct qNode_s
{
	void *firstPtr;

	/* Internal */
	CMapNode *m_mapNode;

	/* Next/Previous node list. First element never has ->prev set and last element never has ->next set */
	struct qNode_s *next;
	struct qNode_s *prev;

	/* Next/Previous selected node list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qNode_s *selectedNext;
	struct qNode_s *selectedPrev;

	char gap2[8];

	struct epair_s *epairs;

	const char *m_nameOverride;
	const char *m_fireEntityOnPass;

	char gap3[4];

	int m_editorFlags;

	char gap4_1[4];

	int m_editorId;

	vec3_t m_vecOrigin;
	vec3_t m_vecAngles;

	char gap5[24];
} qNode_t;
COMPILE_TIME_ASSERT( sizeof( qNode_t ) == 144 );

// clang-format off

// clang-format on

#endif // !PLUGINNODES_H
