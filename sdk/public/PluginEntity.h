//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINENTITY_H )
#define PLUGINENTITY_H

/*
 Entity definition
*/

class CMapEntity;

typedef struct qEntity_s
{
	char gap[8];
	CMapEntity *pMapEntity;
	char gap2[88];
	const char *className;
	char gap3[256];
} qEntity_t;

COMPILE_TIME_ASSERT( sizeof( qEntity_t ) == 368 );

// clang-format off

// clang-format on

#endif // !PLUGINENTITY_H
