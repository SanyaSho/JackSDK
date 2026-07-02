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

#include "BaseTypes.h"

typedef struct epair_s
{
	struct epair_s *next;
	const char *key;
	const char *value;
} epair_t;

typedef struct qSplineNode_s
{
	struct qSplineNode_s *nextNode;
	char gap[8];
	struct qEntity_s *owner;
	char gap2[108];
	float speed;
	float yaw_speed;
	float fov;
} qSplineNode_t;

typedef struct qEntityKeys_s
{
	const char *key;
} qEntityKeys_t;

class CMapEntity;

typedef struct qEntity_s
{
	char gap[8];
	CMapEntity *pMapEntity;
	struct qEntity_s *next;
	char gap2[4];
	rgba_t color;
	char gap2_[56];
	struct epair_s *epairs;
	struct qSplineNode_s *splineNode;
	const char *className;
	char gap4[16];
	int spawnflags;
	int editorFlags;
	char gap5[4];
	int modelType; ///< 3 - mod_studio_hl1
	vec3_t origin;
	vec3_t angles;
	vec3_t bboxMin;
	vec3_t bboxMax;
	char gap6[64];
	qEntityKeys_t entityKeys[14];
} qEntity_t;

COMPILE_TIME_ASSERT( sizeof( qEntity_t ) == 368 );

// clang-format off

// clang-format on

#endif // !PLUGINENTITY_H
