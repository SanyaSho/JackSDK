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


/* This struct is written as a 20 byte blob into the JMF */
typedef struct qPlane_s
{
	vec3_t normal;
	float dist; // DotProduct of normal
	int alignedAxis;
} qPlane_t;
COMPILE_TIME_ASSERT( sizeof( qPlane_t ) == SIZEOF_QPLANE_S /* Always 20 */ );


#define TEXALIGN_NONE			(      0 )
#define TEXALIGN_WORLD			( 1 << 0 )
#define TEXALIGN_FACE			( 1 << 1 )
#define TEXALIGN_QUAKE			( 1 << 2 )

/* This struct is written as a 128 byte blob to the JMF */
typedef struct qTexDef_s
{
	vec3_t m_UAxis;
	float m_xShift;

	vec3_t m_VAxis;
	float m_yShift;

	vec2_t m_scale;

	float m_rotate;

	/* See TEXALIGN_ defines above */
	int m_textureAlignment;

	int m_value; // Quake II "Value"
	int m_lightmapScale; // HL2 addition
	int m_surfaceFlags; // Quake II SurfaceFlags
	int m_contentFlags; // Quake II ContentFlags

	char m_textureName[64];
} qTexDef_t;
COMPILE_TIME_ASSERT( sizeof( qTexDef_t ) == SIZEOF_QTEXDEF_S /* Always 128 */ );


typedef struct qVertex_s
{
	vec3_t coords;
	vec2_t uv;
	int selectionState;
} qVertex_t;
COMPILE_TIME_ASSERT( sizeof( qVertex_t ) == SIZEOF_QVERTEX_S /* Always 24 */ );


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

#if defined( JACK_64BIT )
	char gap1[8];
#else
	char gap1[4];
#endif // JACK_64BIT

	/* Brush this face belongs to */
	struct qBrush_s *m_ownerBrush;

	struct qShader_s *m_shaderInfo;

	char gap2[4];

	int m_editorFlags;
	int m_editorId;

	struct qPlane_s m_plane;

	struct qTexDef_s m_texDef;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	int m_vertexCount;

	struct qVertex_s *m_vertices;
} qFace_t;
COMPILE_TIME_ASSERT( sizeof( qFace_t ) == SIZEOF_QFACE_S );

// clang-format off

// clang-format on

#endif // !PLUGINFACES_H
