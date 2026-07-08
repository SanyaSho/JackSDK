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

class CMapCamera;

typedef struct qCamera_s
{
	char gap1[8];
	CMapCamera *pMapCamera;
	char gap[24];
	int editorFlags;
	char gap2[28];
} qCamera_t;
COMPILE_TIME_ASSERT( sizeof( qCamera_t ) == 72 );

// clang-format off

// clang-format on

#endif // !PLUGINCAMERA_H
