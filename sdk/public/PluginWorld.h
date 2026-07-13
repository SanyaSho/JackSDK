//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Author(-s): SanyaSho (2026)
//
//=====================================================================================//

#if !defined( PLUGINWORLD_H )
#define PLUGINWORLD_H

/*
 Brush-related definitions
*/

#include "BaseTypes.h"

// Need to include PluginFaces.h here becase of qTexDef_s usage below
#include "PluginFaces.h"

struct qGroup_s;
struct qBrush_s;
struct qShader_s;
//struct qTexDef_s;
struct qEntity_s;
struct qNode_s;


class CMapOverlay;

typedef struct qOverlayData_s
{
	vec3_t position;
	vec2_t uv;
	char gap[4];
} qOverlayData_t;
COMPILE_TIME_ASSERT( sizeof( qOverlayData_t ) == 24 );

typedef struct qOverlay_s
{
	void *firstPtr;

	/* Internal */
	CMapOverlay *m_mapOverlay;

	/* Owner of this overlay */
	struct qBrush_s *m_ownerBrush;

	/* Shader used to render this overlay */
	struct qShader_s *m_shaderInfo;

	int m_editorFlags;

	struct qPlane_s m_plane;

	struct qTexDef_s m_texDef;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	/* This field is written as a 96 byte blob into the JMF */
	struct qOverlayData_s m_data[4];
} qOverlay_t;
COMPILE_TIME_ASSERT( sizeof( qOverlay_t ) == 304 );


typedef struct qDecalFragment_s
{
	struct qDecalFragment_s *next;
	struct qDecalFragment_s *prev; // is this ->next?
	struct qDecal_s *m_decalList;
	struct qBrush_s *m_brushOwner;
} qDecalFragment_t;
COMPILE_TIME_ASSERT( sizeof( qDecalFragment_t ) == 32 );


class CMapPatch;

typedef struct qPatchData_s
{
	// This looks like embedded qVertex_t

	vec3_t position;
	vec3_t normal;
	vec2_t uv;
	int unkint1;
} qPatchData_t;
COMPILE_TIME_ASSERT( sizeof( qPatchData_t ) == 36 ); // NOTE: The size is unknown

typedef struct qPatch_s
{
	void *firstPtr;

	/* Internal */
	CMapPatch *m_mapPatch;

	/* Owner of this patch */
	struct qBrush_s *m_ownerBrush;

	/* Shader used to render this patch */
	struct qShader_s *m_shaderInfo;

	struct qTexDef_s m_texDef;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	// cylinder (9x3) -> m_numColumns = 9; m_numRows = 3
	int m_numColumns;
	int m_numRows;
	qPatchData_t m_data[32 /*column*/][32 /*row*/];

	int m_editorFlags;

	/* Internal data used to draw the patch */
	char gap5[4];
	unsigned int m_glBackGeometryCount;
	unsigned int m_glPointsCount;
	char gap6[16];
	const void *m_glBackGeometryIndices;
	const void *m_glPointsIndices;
	char gap7[24];
} qPatch_t;
COMPILE_TIME_ASSERT( sizeof( qPatch_t ) == 37128 );


class CMapBrush;

typedef struct qBrush_s
{
	void *firstPtr;

	/* Internal */
	CMapBrush *m_mapBrush;

	/* Next/Previous brush list. First element never has ->prev set and last element never has ->next set */
	struct qBrush_s *next;
	struct qBrush_s *prev;

	/* Next/Previous selected entity list. First element never has ->selectedPrev and last element never has ->selectedNext */
	struct qBrush_s *selectedNext;
	struct qBrush_s *selectedPrev;

	/* List of groups this brush belongs to */
	struct qGroup_s *m_groupList;
	struct qGroup_s *m_lastGroup;

	/* List of faces of this brush */
	struct qFace_s *m_faceList;
	struct qFace_s *m_lastFace;

	/* Owner entity of this brush (ex. worldspawn) */
	struct qEntity_s *m_ownerEntity;

	struct qPatch_s *m_patch;

	struct qOverlay_s *m_overlay;

	struct qDecalFragment_s *m_decalFragmentList;

	int render_unkint;

	int m_editorFlags;
	int m_editorId;

	/* Number of qPatch_t attached to this brush */
	int m_patchCount;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;
} qBrush_t;
COMPILE_TIME_ASSERT( sizeof( qBrush_t ) == 152 );


typedef struct qTrans_s
{
	char gap[24];
} qTrans_t;
COMPILE_TIME_ASSERT( sizeof( qTrans_t ) == 24 );


typedef struct qSkyData_s
{
	struct qShader_s *m_skyShader;
	vec3_t m_skyAxis;
	float m_skyRotate;
} qSkyData_t;
COMPILE_TIME_ASSERT( sizeof( qSkyData_t ) == 24 );


class CMapWorld;

typedef struct qWorld_s
{
	void *firstPtr;

	/* Internal */
	CMapWorld *m_mapWorld;

	struct qGroup_s *m_groupList;

	/* List of entities this world contains */
	struct qEntity_s *m_entityList;
	struct qEntity_s *m_lastEntity;

	/* List of paths this world contains */
	struct qPath_s *m_pathList;
	struct qPath_s *m_lastPath;

	/* List of cameras this world contains */
	struct qCamera_s *m_cameraList;
	struct qCamera_s *m_lastCamera;

	/* Selection lists. NULL if nothing is selected */
	struct qCamera_s *m_selCamera;
	struct qGroup_s *m_selGroupList;
	struct qEntity_s *m_selEntityList;
	struct qBrush_s *m_selBrushList;
	struct qFace_s *m_selFaceList;
	struct qNode_s *m_selNodeList;

	/* Translucency data for special entities */
	struct qTrans_s *m_transData;

	char unknown_sky_related[8];

	int m_editorFlags;

	vec3_t m_vecCordonMin;
	vec3_t m_vecCordonMax;

	char qWorld_s_gap5[4];

	/* Sky settings */
	struct qSkyData_s m_skyData;
} qWorld_t;
COMPILE_TIME_ASSERT( sizeof( qWorld_t ) == 192 );

// clang-format off

// clang-format on

#endif // !PLUGINWORLD_H
