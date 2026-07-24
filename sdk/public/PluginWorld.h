//=====================================================================================//
//
// Purpose: J.A.C.K. Plugin API
// J.A.C.K. 1.2.4603
//
// Copyright (C) 2026  SanyaSho
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, see <https://www.gnu.org/licenses/>.
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
COMPILE_TIME_ASSERT( sizeof( qOverlayData_t ) == SIZEOF_QOVERLAYDATA_S /* Always 24 */ );

typedef struct qOverlay_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

	/* Internal */
	CMapOverlay *m_mapOverlay;

	/* Owner of this overlay */
	struct qBrush_s *m_ownerBrush;

	/* Shader used to render this overlay */
	struct qShader_s *m_shaderInfo;

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	struct qPlane_s m_plane;

	struct qTexDef_s m_texDef;

	vec3_t m_bboxMin;
	vec3_t m_bboxMax;

	/* This field is written as a 96 byte blob into the JMF */
	struct qOverlayData_s m_data[4];
} qOverlay_t;
COMPILE_TIME_ASSERT( sizeof( qOverlay_t ) == SIZEOF_QOVERLAY_S );


typedef struct qDecalFragment_s
{
	struct qDecalFragment_s *next;
	struct qDecalFragment_s *prev; // is this ->next?
	struct qDecal_s *m_decalList;
	struct qBrush_s *m_brushOwner;
} qDecalFragment_t;
COMPILE_TIME_ASSERT( sizeof( qDecalFragment_t ) == SIZEOF_QDECALFRAGMENT_S );


class CMapPatch;

typedef struct qPatchData_s
{
	// This looks like embedded qVertex_t

	vec3_t position;
	vec3_t normal;
	vec2_t uv;

	int m_flags;
} qPatchData_t;
COMPILE_TIME_ASSERT( sizeof( qPatchData_t ) == SIZEOF_QPATCHDATA_S ); // NOTE: The size is unknown

typedef struct qPatch_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

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

	/* Editor flags. See head of PluginAPI.h for more info */
	/* If you want to check if this patch is selected use m_ownerBrush->m_editorFlags instead */
	int m_editorFlags;

	/* Internal data used to draw the patch */
	unsigned int m_glVertexCount;
	unsigned int m_glBackGeometryCount;
	unsigned int m_glPointsCount;
	unsigned int m_glFrontGeometryCount;
	const void *m_glVertices;
	const void *m_glBackGeometryIndices;
	const void *m_glPointsIndices;
	const void *m_glFrontGeometryIndices;
#if defined( JACK_64BIT )
	char gap7[16];
#else
	char gap7[8];
#endif // JACK_64BIT
} qPatch_t;
COMPILE_TIME_ASSERT( sizeof( qPatch_t ) == SIZEOF_QPATCH_S );


class CMapBrush;

#define BRUSH_EXTRADATA_PATCH		1 /* Will serialize a patch attached to the brush */
#define BRUSH_EXTRADATA_OVERLAY		2 /* Will serialize overlay attached to the brush */

typedef struct qBrush_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

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

	/* Patch attached to this brush (m_extraDataType == 1) */
	struct qPatch_s *m_patch;

	/* Overlay attached to this brush (m_extraDataType == 2) */
	struct qOverlay_s *m_overlay;

	/* List of decals touching this brush */
	struct qDecalFragment_s *m_decalFragmentList;

	/* Used as a lock to tell renderer to not redraw the brush */
	int render_unkint;

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	/* Internal brush ID */
	int m_editorId;

	/* Type of extra data serialized with the brush. See BRUSH_EXTRADATA_ defines above. */
	int m_extraDataType;

	/* Brush bounds */
	vec3_t m_bboxMin;
	vec3_t m_bboxMax;
} qBrush_t;
COMPILE_TIME_ASSERT( sizeof( qBrush_t ) == SIZEOF_QBRUSH_S );


typedef struct qTrans_s
{
	char gap[SIZEOF_QTRANS_S];
} qTrans_t;
COMPILE_TIME_ASSERT( sizeof( qTrans_t ) == SIZEOF_QTRANS_S );


typedef struct qSky_s
{
	char gap[SIZEOF_QSKY_S];
} qSky_t;
COMPILE_TIME_ASSERT( sizeof( qSky_t ) == SIZEOF_QSKY_S );


typedef struct qSkyData_s
{
	struct qShader_s *m_skyShader;
	vec3_t m_skyAxis;
	float m_skyRotate;
} qSkyData_t;
COMPILE_TIME_ASSERT( sizeof( qSkyData_t ) == SIZEOF_QSKYDATA_S ); // NOTE: The size is unknown


class CMapWorld; /* CMapWorld inherits qWorld_s */

typedef struct qWorld_s
{
#if JACK_API_VERSION > API_VERSION_HLFX_FREEWARE
	void *firstPtr;
#endif // JACK_API_VERSION > API_VERSION_HLFX_FREEWARE

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
	struct qTrans_s *m_trans;

	struct qSky_s *m_sky;

	/* Editor flags. See head of PluginAPI.h for more info */
	int m_editorFlags;

	vec3_t m_vecCordonMin;
	vec3_t m_vecCordonMax;

	/* Sky settings */
	struct qSkyData_s m_skyData;
} qWorld_t;
COMPILE_TIME_ASSERT( sizeof( qWorld_t ) == SIZEOF_QWORLD_S );

// clang-format off

// clang-format on

#endif // !PLUGINWORLD_H
