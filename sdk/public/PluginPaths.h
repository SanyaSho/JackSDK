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
 Nodes API
*/

#include "BaseTypes.h"

class CMapPath;

typedef struct qPath_s
{
	char gap[8];
	CMapPath *pMapPath;
	char gap2[32];
	void *world;
	const char *pathName;
	const char *pathClassname;
	int pathDirection;
	int editorFlags;
	char gap3_2[4];
	vec3_t bboxMin;
	vec3_t bboxMax;
	char gap4[4];
} qPath_t;
COMPILE_TIME_ASSERT( sizeof( qPath_t ) == 112 );

// clang-format off

// clang-format on

#endif // !PLUGINPATHS_H
