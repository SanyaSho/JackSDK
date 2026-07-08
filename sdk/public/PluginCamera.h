//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINCAMERA_H )
#define PLUGINCAMERA_H

/*
 Camera API

 A new camera can be created using Camera_Create function.
 To setup camera's position and angles use Camera_Setup function.
 If you don't need to have a camera anymore, call Camera_Destroy with a pointer to previously created camera.
*/

#include "BaseTypes.h"

struct qWorld_s;


class CMapCamera;

typedef struct qCamera_s
{
	char gap[8];

	CMapCamera *m_mapCamera;

	struct qCamera_s *next;
	struct qCamera_s *prev;

	struct qWorld_s *m_ownerWorld;

	int m_editorFlags;

	int m_editorId;

	vec3_t m_vecOrigin;
	vec3_t m_vecAngles;
} qCamera_t;
COMPILE_TIME_ASSERT( sizeof( qCamera_t ) == 72 );

// clang-format off

// clang-format on

#endif // !PLUGINCAMERA_H
