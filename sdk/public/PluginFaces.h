//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINFACES_H )
#define PLUGINFACES_H

/*
 Faces API
*/

#include "BaseTypes.h"

struct qShader_s;


typedef struct qPlane_s
{
	vec3_t normal;
	float dist;
} qPlane_t;
COMPILE_TIME_ASSERT( sizeof( qPlane_t ) == 16 );


typedef struct qTexDef_s
{
	vec3_t rightAxis;

	float xShift;

	vec3_t bottomAxis;

	float yShift;

	vec2_t scale;

	float angle;

	int textureAlignment;

	int valueQuakeII;
	int lightmapScale;
	int unknown_OverlayValue;

	int surfaceFlags;

	char textureName[64];
} qTexDef_t;
COMPILE_TIME_ASSERT( sizeof( qTexDef_t ) == 128 );


typedef struct qVertex_s
{
	vec3_t coords;
	vec2_t uv;
	int selectionState;
} qVertex_t;
COMPILE_TIME_ASSERT( sizeof( qVertex_t ) == 24 );


class CMapFace;

typedef struct qFace_s
{
	char gap_1[8];

	CMapFace *pMapFace;

	struct qFace_s *next;

	char gap[40];

	struct qShader_s *shaderInfo;

	char gap_[4];

	int faceFlags;

	char gap2[4];

	struct qPlane_s plane;
	int alignedAxis;

	struct qTexDef_s texDef;

	char gap3[24];

	int vertexCount;

	char gap4[4];

	struct qVertex_s *vertexData;
} qFace_t;
COMPILE_TIME_ASSERT( sizeof( qFace_t ) == 272 );

#endif // !PLUGINFACES_H
