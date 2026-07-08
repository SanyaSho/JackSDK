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
	char gap4[248];
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
	char gap[8];
	CMapBrush *m_mapBrush;
	struct qBrush_s *next;
	struct qBrush_s *prev;
	char gap2[16];
	struct qGroup_s *m_groupList;
	struct qGroup_s *m_lastGroup;
	struct qFace_s *m_faceList;
	struct qFace_s *m_lastFace;
	struct qEntity_s *m_entim_ownerEntityty;
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


class CMapWorld;

typedef struct qWorld_s
{
	char gap[8];
	CMapWorld *m_mapWorld;
	char gap2[8];
	struct qEntity_s *m_entityList;
	struct qEntity_s *m_lastEntity;
	struct qPath_s *m_pathList;
	struct qPath_s *m_lastPath;
	struct qCamera_s *m_cameraList;
	struct qCamera_s *m_lastCamera;
	char gap3[1024];
} qWorld_t;
//COMPILE_TIME_ASSERT( sizeof( qWorld_t ) == ??? );

// clang-format off

// clang-format on

#endif // !PLUGINWORLD_H
