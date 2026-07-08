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


/* This struct is written as a 128 byte blob to the JMF */
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
	void *firstPtr;

	/* Internal */
	CMapFace *m_mapFace;

	/* Next/Previous face list. First element never has ->prev set and last element never has ->next set */
	struct qFace_s *next;
	struct qFace_s *prev;

	/* Next/Previous selected face list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qFace_s *selectedNext;
	struct qFace_s *selectedPrev;

	char gap1[8];

	/* Brush this face belongs to */
	struct qBrush_s *m_ownerBrush;

	struct qShader_s *m_shaderInfo;

	char gap2[4];

	int m_editorFlags;
	int m_editorId;

	struct qPlane_s m_plane;
	int m_alignedAxis;

	struct qTexDef_s m_texDef;

	char gap4[24];

	int m_vertexCount;

	char gap5[4];

	struct qVertex_s *m_vertices;
} qFace_t;
COMPILE_TIME_ASSERT( sizeof( qFace_t ) == 272 );

// clang-format off

// clang-format on

#endif // !PLUGINFACES_H
