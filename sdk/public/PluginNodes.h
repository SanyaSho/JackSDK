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

// clang-format off

class CMapNode;

typedef struct qNode_s
{
	char gap[8];
	CMapNode *pMapNode;
	struct qNode_s *nextNode;
	struct qNode_s *parentNode;
	char gap2[24];
	struct epair_s *epairs;
	const char *nameOverride;
	const char *fireEntityOnPass;
	char gap3[4];
	int editorFlags;
	char gap4_1[4];
	char gap4_2[4];
	vec3_t nodeOrigin;
	vec3_t nodeAngles;
	char gap5[24];
} qNode_t;

COMPILE_TIME_ASSERT( sizeof( qNode_t ) == 144 );

// clang-format on

#endif // !PLUGINNODES_H
