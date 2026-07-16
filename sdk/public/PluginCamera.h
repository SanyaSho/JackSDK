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
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapCamera *m_mapCamera;

	/* Next/Previous entity in the list. First entity never has ->prev and last entity never has ->next. */
	struct qCamera_s *next;
	struct qCamera_s *prev;

	/* World this camera belongs to */
	struct qWorld_s *m_ownerWorld;

	int m_editorFlags;

	/* Internal camera ID */
	int m_editorId;

	vec3_t m_vecOrigin;
	vec3_t m_vecAngles;
} qCamera_t;
COMPILE_TIME_ASSERT( sizeof( qCamera_t ) == SIZEOF_QCAMERA_S );

// clang-format off

// clang-format on

#endif // !PLUGINCAMERA_H
