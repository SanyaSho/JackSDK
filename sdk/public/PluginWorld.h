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


class CMapOverlay;

typedef struct qOverlay_s
{
	char gap[8];
	CMapOverlay *m_mapOverlay;
	struct qBrush_s *m_ownerBrush;
	char gap2[8];
	int m_editorFlags;
	char data_20bytes[20];
	qTexDef_s m_texDef;
	char gap4[120];
} qOverlay_t;
COMPILE_TIME_ASSERT( sizeof( qOverlay_t ) == 304 );


class CMapPatch;

typedef struct qPatch_s
{
	char gap[8];
	CMapPatch *m_mapPatch;
	struct qBrush_s *m_ownerBrush;
	struct qShader_s *m_shaderInfo;
	struct qTexDef_s m_texDef;
	char gap3[24];
	int unkint1;
	int unkint2;
	char data_36864bytes[36864];
	int m_editorFlags;
	char gap4[68];
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

	char gap4[12];

	int m_editorFlags;
	int m_editorId;

	int m_patchCount;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;
} qBrush_t;
COMPILE_TIME_ASSERT( sizeof( qBrush_t ) == 152 );


typedef struct qSkyData_s
{
	struct qShader_s *m_skyShader;
	vec3_t m_skyAxis;
	float m_skyRotate;
} qSkyData_t;
COMPILE_TIME_ASSERT( sizeof( qSkyData_t ) == 24 );


typedef struct qTrans_s
{
	struct qTrans_s *next;
	char gap[4];
	float m_unknownFloat;
	struct qEntity_s *m_ownerEntity;
} qTrans_t;
COMPILE_TIME_ASSERT( sizeof( qTrans_t ) == 24 );


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
	void *qWorld_s_gap3_1;
	void *m_selectedUnknownList;
	struct qEntity_s *m_selEntityList;
	struct qBrush_s *m_selBrushList;
	struct qFace_s *m_selFaceList;
	void *qWorld_s_gap3_6;

	/* Translucency data for special entities */
	struct qTrans_s *m_transData;

	char unknown_sky_related[8];

	int m_editorFlags;

	char qWorld_s_gap5[28];

	/* Sky settings */
	struct qSkyData_s m_skyData;
} qWorld_t;
COMPILE_TIME_ASSERT( sizeof( qWorld_t ) == 192 );

// clang-format off

// clang-format on

#endif // !PLUGINWORLD_H
